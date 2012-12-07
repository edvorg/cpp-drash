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
    ////////////////////////////////////////////////////////////////////////

    GetEventSystem().SetMode(std::string("editor_figure_creation_mode"));

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

    ////////////////////////////////////////////////////////////////////////

    GetEventSystem().SetMode(std::string("editor_mode"));

    GetEventSystem().SetProcessor("C-c C-c", CAppEventProcessor(
    [this] ()
    {
        CreateTemplate();
    }));

    GetEventSystem().SetProcessor("C-c C-f", CAppEventProcessor(
    [this] ()
    {
        CreateFigure();
    }));

    GetEventSystem().SetProcessor("C-c C-d", CAppEventProcessor(
    [this] ()
    {
        DetachCurrentObject();
    }));
}

void CTestEditorApp::CreateFigure()
{
    if (mCurrentTemplate != nullptr)
    {
        mCurrentTemplate->mFigures.resize(mCurrentTemplate->mFigures.size()+1);

        GetEventSystem().SetMode(std::string("editor_figure_creation_mode"));
    }
}

void CTestEditorApp::CompleteFigure()
{
    if (mCurrentTemplate != nullptr && mVertices.size() >= 3)
    {
        mCurrentTemplate->mFigures.back().mVertices = mVertices;

        for (auto i = mCurrentTemplate->mFigures.back().mVertices.begin();
             i != mCurrentTemplate->mFigures.back().mVertices.end(); i++)
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
        mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);

        mVertices.clear();

        GetEventSystem().SetMode(std::string("editor_mode"));
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
    if (mCurrentObject != nullptr)
    {
        mCurrentObject->SetDynamic(true);
        mCurrentObject = nullptr;
    }
}

} // namespace drash
