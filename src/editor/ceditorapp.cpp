// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "ceditorapp.h"

namespace drash {

bool CObjectEditorApp::Init()
{
    if (CApp::Init() == false) {
        return false;
    }

    auto t = GetTemplateSystem().CreateSceneObjectTemplate("name1");
    if (t != nullptr)
    {
        t->mFigures.resize(1);
        t->mFigures[0].mVertices.push_back(CVec2(-2, -1));
        t->mFigures[0].mVertices.push_back(CVec2(2, -1));
        t->mFigures[0].mVertices.push_back(CVec2(1, 1));
        t->mFigures[0].mVertices.push_back(CVec2(-1, 1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("name2");
    if (t != nullptr)
    {
        t->mFigures.resize(1);
        t->mFigures[0].mVertices.push_back(CVec2(2, 1));
        t->mFigures[0].mVertices.push_back(CVec2(-2, 1));
        t->mFigures[0].mVertices.push_back(CVec2(-1, -1));
        t->mFigures[0].mVertices.push_back(CVec2(1, -1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("ground");
    if (t != nullptr)
    {
        t->mFigures.resize(1);
        t->mFigures[0].mDepth = 10;
        t->mFigures[0].mVertices.push_back(CVec2(50, 5));
        t->mFigures[0].mVertices.push_back(CVec2(-50, 5));
        t->mFigures[0].mVertices.push_back(CVec2(-50, -5));
        t->mFigures[0].mVertices.push_back(CVec2(50, -5));
    }
    SetProcessor();

    return true;
}

void CObjectEditorApp::Step(double _dt)
{
    CApp::Step(_dt);
}

void CObjectEditorApp::Render()
{
    CApp::Render();
    if (mVertexs.size() != 0) {
        for (unsigned int i = 1 ; i < mVertexs.size() ; i++) {
            GetDebugDrawSystem().DrawLine(mVertexs[i-1],mVertexs[i],b2Color(0,255,0));
        }
        GetDebugDrawSystem().DrawLine(mVertexs[mVertexs.size() -1 ],GetCursorPos(),b2Color(0,255,0));
    }
}

void CObjectEditorApp::StartBuild()
{
    mState = BuildState;
}

void CObjectEditorApp::SetProcessor()
{
    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        //LOG_INFO("Click !!!");
        switch (mState) {
            case BuildState:
                mVertexs.push_back(GetCursorPos());
                break;
            case MoveState:
                mOldPositon = GetCursorPos();

                mSelectedFigure = SelectFigure(mOldPositon);

                GetDebugDrawSystem().ScreenSpaceToWorldSpace(mOldPositon,
                                      -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
                if (mSelectedFigure == nullptr) LOG_INFO("NOOOO");
                break;
            case Simple:
                mSelectedFigure = nullptr;
                break;
        }
    },
    [this] ()
    {
        if (mSelectedFigure != nullptr && mState == MoveState) {
            LOG_INFO("Move figure now");
            //LOG_INFO("Move figure now");
            CVec2 position = GetCursorPos();
            GetDebugDrawSystem().ScreenSpaceToWorldSpace(position,
                                  -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
            float disX = position.x - mOldPositon.x;
            float disY = position.y - mOldPositon.y;
            mOldPositon = position;
            //mSelectedFigure->MoveToDistance(disX,disY);
        }
    },
    [this] ()
    {
        if (mState = MoveState) {
            mSelectedFigure = nullptr;
        }
    }
    ));
    /*
     *    [this] ()// left mouse button pressed
    {
        // choose object here
        mSelectedObject = GetDebugDrawSystem().FindObject(GetCursorPos());
    },
    [this] ()// left mouse button is being pressed
    {
        // move object if choosen
        if (mSelectedObject != nullptr)
        {
            CVec2 pos = GetCursorPos();
            GetDebugDrawSystem().ScreenSpaceToWorldSpace(pos, mSelectedObject->GetZ().Get() - GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
            mSelectedObject->SetPos(pos);
        }
    }));
     */
    GetEventSystem().SetProcessor("RB",CAppEventProcessor(
    [this] ()
    {
        BuildFigure(mCurrentTemplateName);
    }
    ));
}

bool CObjectEditorApp::BuildFigure(const std::string &_objectName)
{
    if (mVertexs.size() < 3) {
        return false;
    }
    if (ValidateFigure() == false) {
        LOG_ERR("This figure can't build");
        return false;
    }

    auto obj = GetTemplateSystem().FindTemplate(_objectName);
    if (obj == nullptr) {
        return false;
    }
    CFigureParams param;
    std::for_each(mVertexs.begin() , mVertexs.end() , [this] (CVec2 &v)
    {
        GetDebugDrawSystem().ScreenSpaceToWorldSpace(v,
                 -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
    });
    param.mVertices = mVertexs;
    obj->mFigures.push_back(param);

    ShowObject(_objectName);

    mTreeRefreshHandler();

    mState = Simple;
    mVertexs.clear();

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
//    params.mPos.Set(0);
    auto obj = GetTemplateSystem().CreateSceneObjectFromTemplate(_name,params);
    mCurrentObject = obj;
}


// TODO: Implements this!
bool CObjectEditorApp::ValidateFigure()
{
    return true;
}

void CObjectEditorApp::RemoveCurrentObject()
{
    if (mCurrentObject != nullptr) {
        GetScene().DestroyObject(mCurrentObject);
        mCurrentObject = nullptr;
    }
}

CFigure *CObjectEditorApp::SelectFigure(const CVec2 &_pos)
{
//    CSceneObject *selectedObject = GetDebugDrawSystem().FindObject(_pos);
    if (mCurrentObject != nullptr) {
        return nullptr;
        //return mCurrentObject->FindFigure(_pos,mCurrentObject->GetZ().Get());
    }

    return nullptr;
}


}// namespace drash
