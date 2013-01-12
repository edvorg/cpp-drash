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

#include "test8.h"

#include "../levelmanager/level.h"
#include "../scene/figure.h"
#include "../greng/camera.h"

namespace drash
{

namespace test
{

void CTest8::Release()
{
    GetTemplateSystem().Store();
    mLevel1->Store("level1");
    mLevel2->Store("level2");
    CApp::Release();
}

bool CTest8::Init()
{
    if (CApp::Init() == false ||
        InitUI() == false ||
        InitLevels() == false ||
        InitCamera() == false ||
        InitLights() == false)
    {
        return false;
    }
    GetTemplateSystem().Load();
    return true;
}

bool CTest8::InitUI()
{
    GetUISystem().SetWidth(1024);

    mButton1.Connect(&GetUISystem());
    mButton1.SetPos(CVec2i(5, 30));
    mButton1.SetSize(CVec2ui(100, 20));

    mButton1.SetClickHandler([this] ()
    {
        if (mLevel1 != nullptr)
        {
            GetLevelManager().StartLevel(mLevel1);
        }
    });

    mButton2.Connect(&GetUISystem());
    mButton2.SetPos(CVec2i(5, 5));
    mButton2.SetSize(CVec2ui(100, 20));

    mButton2.SetClickHandler([this] ()
    {
        if (mLevel2 != nullptr)
        {
            GetLevelManager().StartLevel(mLevel2);
        }
    });

    return true;
}

bool CTest8::InitLevels()
{
    mLevel1 = GetLevelManager().CreateLevel();
    mLevel2 = GetLevelManager().CreateLevel();

    mLevel1->Load("level1");
    mLevel2->Load("level2");
    return true;
    if (mLevel1 == nullptr ||
        mLevel2 == nullptr)
    {
        return false;
    }

    CSceneObjectGeometry *g1 = GetTemplateSystem().CreateSceneObjectTemplate("object1_template");
    CSceneObjectGeometry *g2 = GetTemplateSystem().CreateSceneObjectTemplate("object2_template");

    if (g1 == nullptr ||
        g2 == nullptr)
    {
        return false;
    }

    g1->mFigures.resize(1);
    g1->mFigures[0].mDepth = 1;
    g1->mFigures[0].mVertices.push_back(CVec2f(-1, 0));
    g1->mFigures[0].mVertices.push_back(CVec2f(1, 0));
    g1->mFigures[0].mVertices.push_back(CVec2f(0, 1));

    g2->mFigures.resize(1);
    g2->mFigures[0].mDepth = 1;
    g2->mFigures[0].mVertices.push_back(CVec2f(-1, -1));
    g2->mFigures[0].mVertices.push_back(CVec2f(1, -1));
    g2->mFigures[0].mVertices.push_back(CVec2f(1, 1));
    g2->mFigures[0].mVertices.push_back(CVec2f(-1, 1));

    CSceneObjectParams *p1 = mLevel1->AddObject("object1_template", "obj1");
    CSceneObjectParams *p2 = mLevel1->AddObject("object2_template", "obj2");

    if (p1 == nullptr ||
        p2 == nullptr)
    {
        return false;
    }

    p2->mPos.Set(3, 0, 0);

    CSceneObjectParams *p3 = mLevel2->AddObject("object1_template", "obj1");
    CSceneObjectParams *p4 = mLevel2->AddObject("object2_template", "obj2");

    if (p3 == nullptr ||
        p4 == nullptr)
    {
        return false;
    }

    p3->mPos.Set(3, 0, 0);

    return true;
}

bool CTest8::InitCamera()
{
    greng::CCameraParams p;
    p.mPos.Set(0, 0, 10);
    p.mFov = M_PI / 6.0;
    auto c = GetCameraManager().CreateCamera(p);
    GetDebugRenderer().SetCamera(c);

    return true;
}

bool CTest8::InitLights()
{
    mLight1.mPosition.Set(0, 10, 0);
    GetDebugRenderer().SetLight(&mLight1);
    return true;
}

} // namespace test

} // namespace drash
