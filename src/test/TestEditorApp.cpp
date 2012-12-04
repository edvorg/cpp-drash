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

#include "TestEditorApp.h"

#include "../cscene.h"
#include "app/appeventprocessor.h"

#include <sstream>

namespace drash
{


bool CTestEditorApp::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->SetZ(100);

    SetProcessors();

    return true;
}

void CTestEditorApp::Render()
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

void CTestEditorApp::SetProcessors()
{
    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        if (mState == StateFigure)
        {
            mVertices.push_back(GetCursorPos());
        }
    }));

    GetEventSystem().SetProcessor("RB", CAppEventProcessor(
    [this] ()
    {
        if (mState == StateFigure)
        {
            CompleteFigure();
            mState = StateNormal;
        }
    }));

    GetEventSystem().SetProcessor("C-b C-b", CAppEventProcessor(
    [this] ()
    {
        if (mState == StateNormal)
        {
            CreateTemplate();
        }
    }));

    GetEventSystem().SetProcessor("C-b C-f", CAppEventProcessor(
    [this] ()
    {
        if (mState == StateNormal)
        {
            if (mCurrentTemplate != nullptr)
            {
                CreateFigure();
                mState = StateFigure;
            }
        }
        else if (mState == StateFigure)
        {
            CompleteFigure();
            mState = StateNormal;
        }
    }));

    GetEventSystem().SetProcessor("C-b C-d", CAppEventProcessor(
    [this] ()
    {
        DetachCurrentObject();
    }));

    GetEventSystem().SetProcessor("C-x C-c", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));
}

void CTestEditorApp::CreateFigure()
{
    mCurrentTemplate->mGeometry.mFigures.resize(mCurrentTemplate->mGeometry.mFigures.size()+1);
}

void CTestEditorApp::CompleteFigure()
{
    if (mCurrentTemplate != nullptr)
    {
        mCurrentTemplate->mGeometry.mFigures.back().mVertices = mVertices;

        for (auto i = mCurrentTemplate->mGeometry.mFigures.back().mVertices.begin();
             i != mCurrentTemplate->mGeometry.mFigures.back().mVertices.end(); i++)
        {
            GetDebugDrawSystem().ScreenSpaceToWorldSpace(*i, -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
        }

        if (mCurrentObject != nullptr)
        {
            GetScene().DestroyObject(mCurrentObject);
            this->mCurrentObject = nullptr;
        }

        CSceneObjectParams p;
        p.mDynamic = false;
        mCurrentObject = GetTemplateSystem().CreateSceneObjectFromTemplate(mCurrentTemplate->mName.c_str(), p);

        mVertices.clear();
    }
}

void CTestEditorApp::CreateTemplate()
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

void CTestEditorApp::DetachCurrentObject()
{
    if (mState == StateNormal && mCurrentObject != nullptr)
    {
        mCurrentObject->SetDynamic(true);
        mCurrentObject = nullptr;
    }
}

} // namespace drash
