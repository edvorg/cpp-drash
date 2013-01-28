// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "sceneeditorapp.h"
#include "../scene/sceneobject.h"
#include "../levelmanager/level.h"
#include "../diag/logger.h"

#include <QDebug>

using namespace greng;
using namespace std;

namespace drash {

CSceneEditorApp::CSceneEditorApp()
{
}

bool CSceneEditorApp::Init()
{

    if (CApp::Init() == false) {
        return false;
    }

    if (InitCamera() == false ||
        InitPointLight() == false) {
        return false;
    }

    SetProcessors();
    SetCameraProcessors();
    SetDragDropProcessors();
    mTimer.Reset(true);
//    mMoveablePoint.SetSize(100);
    return true;
    //GetTemplateSystem().CreateSceneObjectFromTemplate("Object1",CSceneObjectParams());
}

void CSceneEditorApp::Step(double _dt)
{
    mTimer.Tick();
    if (mPlayLevel == true ){
        CApp::Step(_dt);
    } else {
        CApp::Step(0);
    }
    if (mSelectedObject != nullptr){
        mMoveablePoint.SetCursorPos(GetCursorPos());
        mMoveablePoint.Step(_dt);
    }
}

void CSceneEditorApp::Render()
{
    CApp::Render();
//    if (mPlayLevel == false && mCurrentLevel != nullptr) {
//        for (const auto &headitem : mCurrentLevel->GetObjects()) {
//            string templatename = headitem.first;
//            CSceneObjectGeometry *g = GetTemplateSystem().FindTemplate(templatename);
//            for (const auto &item : headitem.second) {
//                CSceneObjectParams params = item.second;
//                GetDebugRenderer().RenderObject(g,&params);
//            }
//        }
//    }

    if (mSelectedObject != nullptr) {
        mMoveablePoint.Render(GetGrengSystems().GetRenderer());
    }

    RenderDragTemplate();
}

void CSceneEditorApp::Release()
{
    CApp::Release();
}

void CSceneEditorApp::UpdateTemplateSystem()
{
    GetTemplateSystem().Load();
}

bool CSceneEditorApp::LoadLevel(const std::string &_filename)
{
    if (mCurrentLevel != nullptr) {
        GetLevelManager().DestroyLevel(mCurrentLevel);
        mCurrentLevel = nullptr;
    }

    mSelectedObject = nullptr;
    mCurrentLevel = GetLevelManager().CreateLevel();

    if (mCurrentLevel == nullptr) {
        return false;
    }

    if (mCurrentLevel->Load(_filename) == false) {
        return false;
    }

    mFileNameLevel = _filename;

    mTreeRefreshHandler();
    GetLevelManager().StartLevel(mCurrentLevel,mObjectParams);
    mPlayLevel = false;
    return true;
    //GetLevelManager().StartLevel(level);
}

bool CSceneEditorApp::SaveLevelAs(const std::string &_filename)
{
    if (mCurrentLevel == nullptr) {
        return false;
    }

    return mCurrentLevel->Store(_filename);
}

bool CSceneEditorApp::NewLevel()
{
    mSelectedObject = nullptr;
    if (mCurrentLevel != nullptr) {
        GetLevelManager().DestroyLevel(mCurrentLevel);
    }
    mCurrentLevel = GetLevelManager().CreateLevel();
    if (mCurrentLevel == nullptr) {
        return false;
    }
    GetLevelManager().StartLevel(mCurrentLevel);
    return true;
}

void CSceneEditorApp::StartCurrentLevel()
{
    if (mCurrentLevel != nullptr) {
        mPlayLevel = true;
        mSelectedObject = nullptr;
    }
}

void CSceneEditorApp::AddObject(const std::string &_name, const CVec3f &_pos)
{
    if (mCurrentLevel == nullptr) {
        LOG_WARN("Not set current level");
        return;
    }

    CSceneObjectGeometry *obj = GetTemplateSystem().FindTemplate(_name);
    if (obj == nullptr) {
        LOG_ERR(_name.c_str()  << " not found in Template System");
        return;
    }

    qDebug() << "Add object";

    std::ostringstream is;

    is << "object" ;
    if (mCurrentLevel->GetObjects().find(_name) != mCurrentLevel->GetObjects().end()) {
        is << mCurrentLevel->GetObjects().at(_name).size();
    } else {
        is << "0";
    }

    qDebug() << is.str().c_str();
    CSceneObjectParams *p = mCurrentLevel->AddObject(_name,is.str());
    if (p == nullptr) {
        return;
    }
    p->mPos = _pos;
    GetLevelManager().StartLevel(mCurrentLevel,mObjectParams);

    mTreeRefreshHandler();
}

void CSceneEditorApp::SetProcessors()
{
    GetEventSystem().SetProcessor("RB",CAppEventProcessor(
    [this]() {
        if (mPlayLevel == true) {
            return;
        }
        mSelectedObject = SelectObject();
        if (mSelectedObject != nullptr) {
            mOldpositon = mSelectedObject->GetPos();
            mMoveablePoint.SetCenter(mOldpositon);
        } else {
        }
    }
    ));
    GetEventSystem().SetProcessor("LB",CAppEventProcessor(
    [this]() {
        if (mPlayLevel == true) {
            return;
        }
        if (mSelectedObject != nullptr) {
            mMoveablePoint.ClickBegin();
        }
    },
    [this]() {
        if (mPlayLevel == true) {
            return;
        }
        if (mSelectedObject != nullptr) {
            mMoveablePoint.ClickPressing();
            MoveOfAxis();
        }
    },
    [this] () {
        if (mPlayLevel == true) {
            return;
        }
        if (mSelectedObject != nullptr) {
            mMoveablePoint.ClickEnd();
        }
    }
    ));

    GetEventSystem().SetProcessor("C-LB",CAppEventProcessor(
    [this](){
        if (mPlayLevel){
            return;
        }
        CSceneObject * temp = SelectObject();
        if (temp != nullptr) {
            auto iter = mObjectParams.find(temp);
            if (iter != mObjectParams.end()) {
                CSceneObjectParams *p = iter->second;
                p->mDynamic = false;
                ResetLevel();
            }
        }
    }));
}

void CSceneEditorApp::SetCameraProcessors()
{
    GetEventSystem().SetProcessor("MB", CAppEventProcessor(
    [this] ()
    {
        mCamRotFirstClick = GetCursorPos();
    },
    [this] ()
    {
        CVec2f new_pos = GetCursorPos();

        CVec2f rot = mCamera->GetRotation().Get();
        rot.mY -= new_pos.mX - mCamRotFirstClick.mX;
        rot.mX += new_pos.mY - mCamRotFirstClick.mY;

        mCamera->GetRotation().Set(rot);

        mCamRotFirstClick = new_pos;
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        mCamera->Forward(MOVING_SPEED * mTimer.GetDeltaTime());
    },
    [this]{}
    ));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        mCamera->Strafe(MOVING_SPEED * mTimer.GetDeltaTime());
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        mCamera->Forward(-MOVING_SPEED * mTimer.GetDeltaTime());
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        mCamera->Strafe(-MOVING_SPEED * mTimer.GetDeltaTime());
    }));
}

void CSceneEditorApp::SetDragDropProcessors()
{
    GetEventSystem().SetProcessor("DRDP",CAppEventProcessor(
    [this]() {
        mDragTemplateName = mGetSelectedTemplateHandler();
        mDragNow = true;
    },
    [this] () {
    },
    [this]() {
        CPlane plane;
        plane.SetNormal(CVec3f(0, 0, 1));
        plane.SetPoint(CVec3f(0, 0, 0));

        CVec3f position;
        CVec2f cpos = GetCursorPos();
        mCamera->CastRay(cpos, plane, position);
        AddObject(mDragTemplateName,position);

        mDragNow = false;
        mDragTemplateName = "";
    }
    ));
}

bool CSceneEditorApp::InitCamera()
{
    greng::CCameraParams p;
    p.mPos.Set(10,10,10.0f);
    p.mRotation.Set(-M_PI/4, M_PI/4, 0);
    mCamera = GetGrengSystems().GetCameraManager().CreateCamera(p);
    GetDebugRenderer().SetCamera(mCamera);

    mMoveablePoint.SetCamera(mCamera);

    return true;
}

bool CSceneEditorApp::InitPointLight()
{
    mLight1.mPosition.Set(0, 10, 0);
    GetDebugRenderer().SetLight(&mLight1);
    return true;
}

void CSceneEditorApp::StoreParams()
{
    if (mCurrentLevel == nullptr) {
        return;
    }
    mObjectParams.clear();

}

void CSceneEditorApp::RenderDragTemplate()
{
    if ( mDragNow == false ) {
        return;
    }
    if (mDragTemplateName != "") {
        CSceneObjectGeometry * g = GetTemplateSystem().FindTemplate(mDragTemplateName);
        if (g == nullptr) {
            return;
        }

        CSceneObjectParams params;
        CVec3f position;
        CVec2f cpos = GetCursorPos();

        CPlane plane;
        plane.SetNormal(CVec3f(0, 0, 1));
        plane.SetPoint(CVec3f(0, 0, 0));

//        LOG_INFO(cpos);
        mCamera->CastRay(cpos, plane, position);
//        v = vv;
        params.mPos = position;
//        LOG_INFO(position);
        GetDebugRenderer().RenderObject(*g,params);
    }
}

CSceneObject *CSceneEditorApp::SelectObject()
{
    return GetDebugRenderer().FindObject(mCamera,GetCursorPos());

    //CVec3f cursorpos;
    //CPlane plane;
    //mCamera->CastRay(GetCursorPos(),);
//    for (const auto &headitem : mCurrentLevel->GetObjects()) {
//        //string templatename = headitem.first;
//        //CSceneObjectGeometry *g = GetTemplateSystem().FindTemplate(templatename);
//        for (const auto &item : headitem.second) {
//            CSceneObjectParams params = item.second;

//            //GetDebugRenderer().RenderObject(g,&params);
//        }
//    }
}

void CSceneEditorApp::MoveOfAxis()
{
    mSelectedObject->SetPos(mMoveablePoint.GetCenter());
    mOldpositon = mMoveablePoint.GetCenter();
    auto iter = mObjectParams.find(mSelectedObject);
    if (iter != mObjectParams.end()) {
        CSceneObjectParams *p = iter->second;
        p->mPos = mSelectedObject->GetPos();
    }
}

void CSceneEditorApp::StopLevel()
{
    mPlayLevel = false;
    ResetLevel();
//    GetLevelManager().StartLevel(mCurrentLevel,mObjectParams);
}

void CSceneEditorApp::ResetLevel()
{
    if (mCurrentLevel != nullptr) {
        GetLevelManager().StartLevel(mCurrentLevel,mObjectParams);
    }
}

void CSceneEditorApp::LookObject(const std::string &_templatename, const std::string &_objectname)
{
    if (mCurrentLevel != nullptr) {
        auto headiter = mCurrentLevel->GetObjects().find(_templatename);
        if (headiter != mCurrentLevel->GetObjects().end()) {
            auto iter = headiter->second.find(_objectname);
            if (iter != headiter->second.end()) {
                CSceneObjectParams p = iter->second;
                mCamera->LookAt(p.mPos);
            }
        }
    }
}

} // namspace drash
