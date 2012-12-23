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

#include "../debugdrawsystem/camera.h"
#include "../misc/plane.h"

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
        t->mFigures[0].mVertices.push_back(CVec2f(-2, -1));
        t->mFigures[0].mVertices.push_back(CVec2f(2, -1));
        t->mFigures[0].mVertices.push_back(CVec2f(1, 1));
        t->mFigures[0].mVertices.push_back(CVec2f(-1, 1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("name2");
    if (t != nullptr)
    {
        t->mFigures.resize(1);
        t->mFigures[0].mVertices.push_back(CVec2f(2, 1));
        t->mFigures[0].mVertices.push_back(CVec2f(-2, 1));
        t->mFigures[0].mVertices.push_back(CVec2f(-1, -1));
        t->mFigures[0].mVertices.push_back(CVec2f(1, -1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("ground");
    if (t != nullptr)
    {
        t->mFigures.resize(1);
        t->mFigures[0].mDepth = 10;
        t->mFigures[0].mVertices.push_back(CVec2f(50, 5));
        t->mFigures[0].mVertices.push_back(CVec2f(-50, 5));
        t->mFigures[0].mVertices.push_back(CVec2f(-50, -5));
        t->mFigures[0].mVertices.push_back(CVec2f(50, -5));
    }
    SetProcessors();

    return true;
}

void CObjectEditorApp::Step(double _dt)
{
    CApp::Step(_dt);
}

void CObjectEditorApp::Render()
{
    CApp::Render();
    if (mVertexs.size() != 0 && mState == BuildState) {
        for (unsigned int i = 1 ; i < mVertexs.size() ; i++) {
            GetDebugDrawSystem().DrawLine(mVertexs[i-1],mVertexs[i],b2Color(0,255,0));
        }
        GetDebugDrawSystem().DrawLine(mVertexs[mVertexs.size() -1 ],GetCursorPos(),b2Color(0,255,0));
        GetDebugDrawSystem().DrawLine(mVertexs[0],GetCursorPos(),b2Color(0,255,0));
    }
    if (mState == StretchState) {
//        qDebug() << "THIS!!!";
//        for (auto iter = mFigurePoints.begin() ; iter != mFigurePoints.end() ; iter++){
//            CVec2f position = **iter;
//            float depth = drash::math::Abs(mCurrentObject->GetPos().Get().mZ
//                        -GetDebugDrawSystem().GetActiveCam()->GetPos().Get().mZ);
//            GetDebugDrawSystem().WorldSpaceToScreenSpace(position,depth);
////                qDebug() << position.x << " " << position.y;
//            GetDebugDrawSystem().DrawPoint(position,10,b2Color(255,155,0));
//        }
        for (unsigned int i = 0 ; i < mCurrentObject->EnumFigures() ; i++ ) {
            CFigure *figure = mCurrentObject->GetFigures()[i];
            for (unsigned int j = 0 ; j < figure->EnumVertices() ; j++)
            {
                CVec3f position(figure->GetVertices()[j], mCurrentObject->GetPos().Get().mZ + figure->GetZ() + figure->GetDepth() * 0.5f);

                b2Color color(255,155,0);

                CVec3f cursor_pos;

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(position);

                GetDebugDrawSystem().CastRay(GetCursorPos(), plane, cursor_pos);

                if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1 &&
                        drash::math::Abs(position.mY -cursor_pos.mY) <= 1)
                {
                    color.Set(255,0,0);
                }

                GetDebugDrawSystem().DrawPoint(position, 10.0f, color);

                //

                color.Set(255,155,0);

                position.mZ = mCurrentObject->GetPos().Get().mZ + figure->GetZ() - figure->GetDepth() * 0.5f;

                plane.SetPoint(position);

                GetDebugDrawSystem().CastRay(GetCursorPos(), plane, cursor_pos);

                if (drash::math::Abs(position.mX -cursor_pos.mX) <= 1 &&
                        drash::math::Abs(position.mY -cursor_pos.mY) <= 1)
                {
                    color.Set(255,0,0);
                }

                GetDebugDrawSystem().DrawPoint(position, 10.0f, color);
            }
        }
    }
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
        LOG_INFO("Click !!! " << mState);
        switch ( mState ) {
            case BuildState:{
                if (mCurrentObject != nullptr)
                    mVertexs.push_back(GetCursorPos());
                break;
            }
            case MoveState:{
                mSelectedFigure = SelectFigure(GetCursorPos());

                CPlane plane;
                plane.SetPoint(CVec3f(0, 0, 0));
                plane.SetNormal(CVec3f(0, 0, 1));

                GetDebugDrawSystem().CastRay(GetCursorPos(), plane, mOldPositionCursor);

                if (mSelectedFigure == nullptr)
                    LOG_INFO("NOOOO");

                break;
            }
            case StretchState:{
                //mCurrentFigureVertex = GetCursorPos();
                SelectVertex();
                //StretchFigure();
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
            LOG_INFO("Move figure now");
            MoveFigure();
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
        if (mState == StretchState) {
            mSelectedFigure = nullptr;
            SaveCurrentObject();
            mVertexIndex = -1;
        }
    }
    ));

    GetEventSystem().SetProcessor("RB",CAppEventProcessor(
    [this] ()
    {
        switch (mState) {
            case BuildState:
                BuildFigure(mCurrentTemplateName);
                break;
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

bool CObjectEditorApp::BuildFigure(const std::string &_objectName)
{
    if (mVertexs.size() < 3 || mCurrentObject == nullptr) {
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
    std::for_each(mVertexs.begin() , mVertexs.end() , [this] (CVec2f &v)
    {
        CPlane plane;
        plane.SetNormal(CVec3f(0, 0, 1));
        plane.SetPoint(CVec3f(0, 0, 0));

        CVec3f vv;
        GetDebugDrawSystem().CastRay(v, plane, vv);
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

CFigure *CObjectEditorApp::SelectFigure(const CVec2f &_pos)
{
    if (mCurrentObject != nullptr) {
        return GetDebugDrawSystem().FindFigure(_pos);
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

    GetDebugDrawSystem().CastRay(GetCursorPos(), plane, pos);

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

            GetDebugDrawSystem().CastRay(posCur, plane, pos);

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
}

void CObjectEditorApp::SelectVertex()
{
    for (unsigned int i = 0 ; i < mCurrentObject->EnumFigures() ; i++ )
    {
        CFigure *figure = mCurrentObject->GetFigures()[i];

        for (unsigned int j = 0 ; j < figure->EnumVertices() ; j++)
        {
            CVec3f position(figure->GetVertices()[j], mCurrentObject->GetPos().Get().mZ + figure->GetZ() + figure->GetDepth() * 0.5f);

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(position);

            CVec3f cursor_pos;

            GetDebugDrawSystem().CastRay(GetCursorPos(), plane, cursor_pos);

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
                position.mZ = mCurrentObject->GetPos().Get().mZ + figure->GetZ() - figure->GetDepth() * 0.5f;

                plane.SetPoint(position);

                GetDebugDrawSystem().CastRay(GetCursorPos(), plane, cursor_pos);

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

    GetTemplateSystem().ChangeGeometry( mCurrentObject->GetGeometry(), mCurrentTemplateName );

    ShowObject(mCurrentTemplateName);
}

float CObjectEditorApp::GetCurDepth()
{
    float depth = drash::math::Abs(mCurrentObject->GetPos().Get().mZ
       -GetDebugDrawSystem().GetActiveCam()->GetPos().Get().mZ);
    return depth;
}


}// namespace drash
