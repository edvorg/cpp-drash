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

#include "test1.h"

#include "../scene.h"
#include "app/appeventprocessor.h"
#include "../debugdrawsystem/camera.h"
#include "../sceneobjects/figure.h"
#include "../sceneobjects/sceneobject.h"

#include <sstream>

#include "../misc/plane.h"

namespace drash
{

namespace test
{

bool CTest1::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 0, 100));

    SetProcessors();

    return true;
}

void CTest1::Render()
{
    CApp::Render();

    if (mVertices.size())
    {
        for (int i = 0; i < (int)mVertices.size() - 1; i++)
        {
            GetDebugDrawSystem().DrawLine(mVertices[i], mVertices[i+1], b2Color(0, 1, 0));
        }
        GetDebugDrawSystem().DrawLine(mVertices[mVertices.size()-1], GetCursorPos(), b2Color(0, 1, 0));
        GetDebugDrawSystem().DrawLine(mVertices[0], GetCursorPos(), b2Color(0, 1, 0));
    }
}

void CTest1::SetProcessors()
{
    GetEventSystem().SetMode(std::string("editor_mode"));

    GetEventSystem().SetProcessor("C-c", CAppEventProcessor(
    [this] ()
    {
        CreateTemplate();
    }));

    GetEventSystem().SetProcessor("C-d", CAppEventProcessor(
    [this] ()
    {
        DetachCurrentObject();
    }));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        mVertices.push_back(GetCursorPos());
    }));

    GetEventSystem().SetProcessor("RB", CAppEventProcessor(
    [this] ()
    {
        CompleteFigure();
    }));

    GetEventSystem().SetProcessor("MB", CAppEventProcessor(
    [this] ()
    {
        mCamRotFirstClick = GetCursorPos();
    },
    [this] ()
    {
        CVec2f new_pos = GetCursorPos();

        CVec2f rot = GetDebugDrawSystem().GetActiveCam()->GetRotation().Get();
        rot.mY -= new_pos.mX - mCamRotFirstClick.mX;
        rot.mX += new_pos.mY - mCamRotFirstClick.mY;

        GetDebugDrawSystem().GetActiveCam()->GetRotation().Set(rot);

        mCamRotFirstClick = new_pos;
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Forward(5);
    }));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Strafe(5);
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Forward(-5);
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Strafe(-5);
    }));

    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));
}

void CTest1::CompleteFigure()
{
    if (mCurrentTemplate != nullptr && mVertices.size() >= 3)
    {        
        mCurrentTemplate->mFigures.resize(mCurrentTemplate->mFigures.size()+1);

        mCurrentTemplate->mFigures.back().mVertices = mVertices;

        for (auto i = mCurrentTemplate->mFigures.back().mVertices.begin();
             i != mCurrentTemplate->mFigures.back().mVertices.end(); i++)
        {
            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, 0));

            CVec3f pos;

            GetDebugDrawSystem().CastRay(*i, plane, pos);

            *i = pos;
        }

        if (mCurrentObject != nullptr)
        {
            GetScene().DestroyObject(mCurrentObject);
            this->mCurrentObject = nullptr;
        }

        CSceneObjectParams p;
        p.mDynamic = false;
        mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);

        mVertices.clear();
    }
}

void CTest1::CreateTemplate()
{
    std::ostringstream is;
    is<<"new_template_"<<(mTemplateCounter++);

    this->mCurrentTemplate = GetTemplateSystem().CreateSceneObjectTemplate(is.str().c_str());

    if (mCurrentObject != nullptr)
    {
        GetScene().DestroyObject(mCurrentObject);
        this->mCurrentObject = nullptr;
    }
}

void CTest1::DetachCurrentObject()
{
    if (mCurrentObject != nullptr)
    {
        mCurrentObject->SetDynamic(true);
        mCurrentObject = nullptr;
    }
}

} // namespace test

} // namespace drash
