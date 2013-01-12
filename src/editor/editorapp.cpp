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

#include "editorapp.h"

#include <QDebug>

#include "../greng/camera.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"

using namespace greng;

namespace drash {

// const
const float CObjectEditorApp::MOVING_SPEED = 60.0f;


bool CObjectEditorApp::Init()
{
    if (CApp::Init() == false) {
        return false;
    }

    SetProcessors();
    SetCameraProcessors();
    GetTemplateSystem().Load();

    CCameraParams cp;
    cp.mPos.Set(10,10,10.0f);
    cp.mRotation.Set(-M_PI/4, M_PI/4, 0);
    mCamera = GetCameraManager().CreateCamera(cp);


    if (mCamera == nullptr)
    {
        return false;
    }

    this->GetDebugRenderer().SetCamera(mCamera);

    greng::CVertexShader *v = GetVertexShaderManager().CreateShaderFromFile("shaders/shader2.120.vs");
    greng::CFragmentShader *f = GetFragmentShaderManager().CreateShaderFromFile("shaders/shader2.120.fs");

    CShaderProgram* shaderProgram = GetShaderProgramManager().CreateProgram(v, f);

    if (shaderProgram == nullptr)
    {
        return false;
    }

    GetDebugRenderer().SetShaderProgram(shaderProgram);

    mPointLight.mPosition = GetCamera()->GetPos();

    GetDebugRenderer().SetLight(&mPointLight);

    mTreeRefreshHandler();
    mMoveablePoint.SetCamera(mCamera);

    mTimer.Reset(true);

    return true;
}

void CObjectEditorApp::Step(double _dt)
{
    CApp::Step(0);

    mTimer.Tick();

    mPointLight.mPosition = GetCamera()->GetPos();

    if (mState == MoveOfAxisState && mSelectedFigure != nullptr) {
        mMoveablePoint.SetCursorPos(GetCursorPos());
        mMoveablePoint.Step(_dt);
    }

}

void CObjectEditorApp::Render()
{
    CApp::Render();
    if (mVertexs.size() != 0 && mState == BuildState) {
        for (unsigned int i = 1 ; i < mVertexs.size() ; i++) {
            GetRenderer().DrawLine(mVertexs[i-1],mVertexs[i], 1, CColor4f(0, 1, 0, 1));
        }
        GetRenderer().DrawLine(mVertexs[mVertexs.size() -1 ],GetCursorPos(), 1, CColor4f(0, 1, 0, 1));
        GetRenderer().DrawLine(mVertexs[0],GetCursorPos(), 1, CColor4f(0, 1, 0, 1));
    }
    if (mState == StretchState && mCurrentObject != nullptr) {
        for (unsigned int i = 0 ; i < mCurrentObject->EnumFigures() ; i++ ) {
            CFigure *figure = mCurrentObject->GetFigures()[i];
            for (unsigned int j = 0 ; j < figure->EnumVertices() ; j++)
            {
                CVec3f position(figure->GetVertices()[j],
                                mCurrentObject->GetPosZ() + figure->GetZ() + figure->GetDepth() * 0.5f);

                CColor4f color(1, 0.5, 0, 1);

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(position);

                CVec3f cursor_pos;

                mCamera->CastRay(GetCursorPos(), plane, cursor_pos);
                if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1 &&
                        drash::math::Abs(position.mY -cursor_pos.mY) <= 1)
                {
                    color.Col3().Set(255,0,0);
                }

                GetRenderer().DrawPoint(mCamera,position, 10.0f, color);

                color.Col3().Set(255,155,0);

                position.mZ = mCurrentObject->GetPosZ() + figure->GetZ() - figure->GetDepth() * 0.5f;

                plane.SetPoint(position);

                mCamera->CastRay(GetCursorPos(), plane, cursor_pos);

                if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1 &&
                        drash::math::Abs(position.mY -cursor_pos.mY) <= 1)
                {
                    color.Col3().Set(255,0,0);
                }

                GetRenderer().DrawPoint(mCamera,position, 10.0f, color);
            }
        }
    }

    if (mState == MoveOfAxisState && mSelectedFigure != nullptr) {
        mMoveablePoint.Render(GetRenderer());
    }
}

void CObjectEditorApp::Release()
{
    GetTemplateSystem().Store();
    CApp::Release();
}

void CObjectEditorApp::StartBuild()
{
    mState = BuildState;
    ChangeMode();
}

void CObjectEditorApp::SetProcessors()
{

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
//        LOG_INFO("Click !!! " << mState);
        switch ( mState ) {
            case BuildState:{
                if (mCurrentObject != nullptr)
                    mVertexs.push_back(GetCursorPos());
                if ( IsConvex() == false ) {
                    mVertexs.pop_back();
                }
                break;
            }
            case MoveState:{
                mSelectedFigure = SelectFigure(GetCursorPos());

                CPlane plane;
                plane.SetPoint(CVec3f(0, 0, 0));
                plane.SetNormal(CVec3f(0, 0, 1));

                mCamera->CastRay(GetCursorPos(), plane, mOldPositionCursor);
                break;
            }
            case StretchState:{
                SelectVertex();
                break;
            }
            case MoveOfAxisState: {
                if (mSelectedFigure != nullptr) {
                    mMoveablePoint.ClickBegin();
                }
                break;
            }
            case Simple:
                mSelectedFigure = nullptr;
                break;
        }
    },
    [this] ()
    {
        if (mSelectedFigure != nullptr && mState == MoveState) {
            MoveFigure();
        }

        if (mSelectedFigure != nullptr && mState == MoveOfAxisState) {
            mMoveablePoint.ClickPressing();
            MoveOfAxis();
        }

        if (mState == StretchState) {
            StretchFigure();
        }
    },
    [this] ()
    {
        if (mState == MoveState) {
            mSelectedFigure = nullptr;
            SaveCurrentObject();
        }
        if (mState == MoveOfAxisState && mSelectedFigure != nullptr) {
            mMoveablePoint.ClickEnd();
            SaveCurrentObject();
        }
        if (mState == StretchState) {
            mSelectedFigure = nullptr;
            SaveCurrentObject();
            mVertexIndex = -1;
        }
        mTreeRefreshHandler();
    }
    ));

    GetEventSystem().SetProcessor("RB",CAppEventProcessor(
    [this] ()
    {
        switch (mState) {
            case BuildState:
                BuildFigure(mCurrentTemplateName);
                break;
            case MoveOfAxisState:{
                mSelectedFigure = SelectFigure(GetCursorPos());

//                if (mSelectedFigure == nullptr)
//                    LOG_INFO("NOOOO 2");
                SettingCenterFigure();
                break;
            }
            case MoveState:
                break;
            case StretchState:
                break;
            case Simple:
                break;
        }
    }
    ));
}

void CObjectEditorApp::SetCameraProcessors()
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

bool CObjectEditorApp::BuildFigure(const std::string &_objectName)
{
    if (mVertexs.size() < 3 || mCurrentObject == nullptr) {
        return false;
    }
    if (ValidateFigure() == false) {
        std::reverse(mVertexs.begin() , mVertexs.end());
        if (ValidateFigure() == false) {
            LOG_ERR("This figure can't build, canceled");
            return false;
        }
    }

    auto obj = GetTemplateSystem().FindTemplate(_objectName);
    if (obj == nullptr) {
        return false;
    }

    CFigureParams param;
    std::for_each(mVertexs.begin() , mVertexs.end() , [this] (CVec2f &v)
    {
        CPlane plane;
        plane.SetNormal(CVec3f(0, 0, 1));
        plane.SetPoint(CVec3f(0, 0, 0));

        CVec3f vv;
        mCamera->CastRay(v, plane, vv);
        v = vv;
    });
    param.mVertices = mVertexs;
    obj->mFigures.push_back(param);

    ShowObject(_objectName);

    mTreeRefreshHandler();

    ActiveMoveMode();
    return true;
}

void CObjectEditorApp::AddNewObjectToTemplate(const std::string &_name)
{
    GetTemplateSystem().CreateSceneObjectTemplate(_name);
    ShowObject(_name);
}

void CObjectEditorApp::ShowObject(const std::string &_name)
{
    RemoveCurrentObject();
    SetCurrentTemplateName(_name);
    CSceneObjectParams params;
    auto obj = GetTemplateSystem().CreateSceneObjectFromTemplate(_name,params);
    mCurrentObject = obj;
}

bool CObjectEditorApp::ValidateFigure()
{
    if (mVertexs.size() < 3 ) {
        return false;
    }

    /// Box2d code

    float area = 0.0f;

    CVec2f pRef(0.0f, 0.0f);

    std::vector<drash::CVec2f> & vs = mVertexs;

    for (unsigned int i = 0; i < vs.size(); ++i)
    {
            CVec2f p1 = pRef;
            CVec2f p2 = vs[i];
            CVec2f p3 = i + 1 < vs.size() ? vs[i+1] : vs[0];

            CVec2f e1 = p2 - p1;
            CVec2f e2 = p3 - p1;
            float D = e1.mX * e2.mY - e1.mY * e2.mX;
            float triangleArea = 0.5f * D;
            area += triangleArea;
    }
    const float espilion = 1.19209289550781250000e-7F;

    if (area <= espilion) {
        return false;
    }

    return true;
}

void CObjectEditorApp::RemoveCurrentObject()
{
    if (mCurrentObject != nullptr) {
        GetScene().DestroyObject(mCurrentObject);
        mCurrentObject = nullptr;
        mTreeRefreshHandler();
    }

}

CFigure *CObjectEditorApp::SelectFigure(const CVec2f &_pos)
{
    if (mCurrentObject != nullptr)
    {
        return GetDebugRenderer().FindFigure(mCamera, _pos);
    }

    return nullptr;
}

void CObjectEditorApp::MoveFigure()
{
    if (mSelectedFigure == nullptr) {
        return;
    }

    CVec3f pos;

    CPlane plane;
    plane.SetNormal(CVec3f(0, 0, 1));
    plane.SetPoint(CVec3f(0, 0, 0));

    mCamera->CastRay(GetCursorPos(), plane, pos);

    float disX = pos.mX - mOldPositionCursor.mX;
    float disY = pos.mY - mOldPositionCursor.mY;

    mOldPositionCursor = pos;

    const CVec2f* v = mSelectedFigure->GetVertices();
    CVec2f* new_vertices = new CVec2f[mSelectedFigure->EnumVertices()];
    for (unsigned int i = 0; i < mSelectedFigure->EnumVertices(); i++)
    {
        new_vertices[i] = v[i];
        new_vertices[i].mX += disX;
        new_vertices[i].mY += disY;
    }

    mSelectedFigure->SetVertices(new_vertices, mSelectedFigure->EnumVertices());
    delete[] new_vertices;
}

void CObjectEditorApp::StretchFigure()
{
    if (mVertexIndex == -1) {
        return;
    }
    CVec2f *ver = new CVec2f[mSelectedFigure->EnumVertices()];
    for (unsigned int i = 0 ; i < mSelectedFigure->EnumVertices() ; i++)
    {
        if (i == (unsigned int)mVertexIndex)
        {
            CVec2f posCur = GetCursorPos();
            qDebug() << posCur.mX << " " << posCur.mY;

            float tmp = mSelectedFigure->GetDepth() * 0.5;

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, mFrontSide ? tmp : -tmp));

            CVec3f pos;

            mCamera->CastRay(posCur, plane, pos);

            ver[i].Set(pos.mX,pos.mY);
        }
        else
        {
            ver[i] = mSelectedFigure->GetVertices()[i];
        }
    }
    mSelectedFigure->SetVertices(ver,mSelectedFigure->EnumVertices());
    delete[] ver;
}

void CObjectEditorApp::ChangeMode()
{
    mVertexs.clear();
    mSelectedFigure = nullptr;
    SaveCurrentObject();
}

void CObjectEditorApp::SelectVertex()
{
    for (unsigned int i = 0 ; i < mCurrentObject->EnumFigures() ; i++ )
    {
        CFigure *figure = mCurrentObject->GetFigures()[i];

        for (unsigned int j = 0 ; j < figure->EnumVertices() ; j++)
        {
            CVec3f position(figure->GetVertices()[j], mCurrentObject->GetPosZ() + figure->GetZ() + figure->GetDepth() * 0.5f);

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(position);

            CVec3f cursor_pos;

            mCamera->CastRay(GetCursorPos(), plane, cursor_pos);

            if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1.0f &&
                    drash::math::Abs(position.mY -cursor_pos.mY) <= 1.0f)
            {
                mCurrentFigureVertex = position;
                mVertexIndex = j;
                mSelectedFigure = figure;
                mFrontSide = true;
                break;
            }
            else
            {
                position.mZ = mCurrentObject->GetPosZ() + figure->GetZ() - figure->GetDepth() * 0.5f;

                plane.SetPoint(position);

                mCamera->CastRay(GetCursorPos(), plane, cursor_pos);

                if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1.0f &&
                        drash::math::Abs(position.mY -cursor_pos.mY) <= 1.0f)
                {
                    mCurrentFigureVertex = position;
                    mVertexIndex = j;
                    mSelectedFigure = figure;
                    mFrontSide = false;
                    break;
                }
            }
        }
    }
}

void CObjectEditorApp::SettingCenterFigure()
{
    if (mSelectedFigure == nullptr) {
        return;
    }

    CVec3f center;

    center.Set(mSelectedFigure->GetVertices()[0], mCurrentObject->GetPosZ() + mSelectedFigure->GetZ());

    for (unsigned int i = 1; i < mSelectedFigure->EnumVertices(); i++)
    {
        center.Vec2() += mSelectedFigure->GetVertices()[i];
    }

    center.Vec2() /= CVec2f(mSelectedFigure->EnumVertices());

    mMoveablePoint.SetCenter(center);
    mOldCenterFigure = center;
}

void CObjectEditorApp::MoveOfAxis()
{
    if (mSelectedFigure == nullptr) {
        return;
    }
    CVec3f pos;

    CPlane plane;
    plane.SetNormal(CVec3f(0, 0, 1));
    plane.SetPoint(CVec3f(0, 0, 0));

    mCamera->CastRay(GetCursorPos(), plane, pos);
    CVec3f newCenter = mMoveablePoint.GetCenter();
    float disX = newCenter.mX - mOldCenterFigure.mX;
    float disY = newCenter.mY - mOldCenterFigure.mY;
    mOldPositionCursor = pos;

    const CVec2f* v = mSelectedFigure->GetVertices();
    CVec2f* new_vertices = new CVec2f[mSelectedFigure->EnumVertices()];
    for (unsigned int i = 0; i < mSelectedFigure->EnumVertices(); i++)
    {
        new_vertices[i] = v[i];
        new_vertices[i].mX += disX;
        new_vertices[i].mY += disY;
    }

    mSelectedFigure->SetVertices(new_vertices, mSelectedFigure->EnumVertices());
    delete[] new_vertices;

    mSelectedFigure->SetZ(mSelectedFigure->GetZ() + newCenter.mZ - mOldCenterFigure.mZ);
    mOldCenterFigure = newCenter;
}


bool CObjectEditorApp::IsConvex() const
{
    if (mVertexs.size() <= 3)
        return true;

    int res = math::Sign((mVertexs[1].mX - mVertexs[0].mX) * (mVertexs[2].mY-mVertexs[0].mY) -
            (mVertexs[2].mX - mVertexs[0].mX) * (mVertexs[1].mY-mVertexs[0].mY) );

    const int N = mVertexs.size();

    for (int i = 1; i < N; i++){
        float buf = (mVertexs[(i+1)%N].mX - mVertexs[i].mX) * (mVertexs[(i+2)%N].mY-mVertexs[i].mY) -
                (mVertexs[(i+2)%N].mX - mVertexs[i].mX) * (mVertexs[(i+1)%N].mY-mVertexs[i].mY);
        if ( res != math::Sign(buf) ) {
            return false;
        }
    }

    return true;
}

void CObjectEditorApp::ActiveStretchMode()
{
    mState = StretchState;
    ChangeMode();
}

void CObjectEditorApp::SaveCurrentObject()
{
    if (mCurrentObject == nullptr) {
        return;
    }

    CSceneObjectGeometry * geometry = new CSceneObjectGeometry();
    mCurrentObject->DumpGeometry(geometry);
    GetTemplateSystem().ChangeGeometry( geometry, mCurrentTemplateName );

    ShowObject(mCurrentTemplateName);
}

float CObjectEditorApp::GetCurDepth()
{
    float depth = drash::math::Abs(mCurrentObject->GetPosZ() -
                                   mCamera->GetPos().Get().mZ);
    return depth;
}


}// namespace drash
