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

#include "ctestapp4.h"

#include <sceneobjects.h>

namespace drash
{

bool CTestApp4::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    auto t = GetTemplateSystem().CreateSceneObjectTemplate("name1");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, 1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("name2");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, -1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("ground");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mDepth = 10;
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, -5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, -5));
    }

    CSceneObjectParams p1;
    p1.mDynamic = false;
    p1.mPos.y = -10;

    return true;
}

void CTestApp4::Step(double _dt)
{
    CApp::Step(_dt);

    mTime += _dt;

    if (mTime >= 1.0)
    {
        CSceneObjectParams params;
        params.mDynamic = true;
        params.mPos.RandX(-20, 20);
        params.mPos.y = 30;

        auto o = GetTemplateSystem().CreateSceneObjectFromTemplate(rand()%2 == 0 ? "name1" : "name2", params);

        CFigureParams f1;
        f1.mDepth = 1;
        f1.mVertices.push_back(CVec2(0,0));
        f1.mVertices.push_back(CVec2(2,0));
        f1.mVertices.push_back(CVec2(2,2));
        f1.mVertices.push_back(CVec2(0,2));
        CFigureParams f2;
        f2.mDepth = 1;
        f2.mVertices.push_back(CVec2(-2,0));
        f2.mVertices.push_back(CVec2(0,0));
        f2.mVertices.push_back(CVec2(0,2));
        f2.mVertices.push_back(CVec2(-2,2));
        CFigureParams f3;
        f3.mDepth = 1;
        f3.mVertices.push_back(CVec2(0,-2));
        f3.mVertices.push_back(CVec2(2,-2));
        f3.mVertices.push_back(CVec2(2,0));
        f3.mVertices.push_back(CVec2(0,0));
        CFigureParams f4;
        f4.mDepth = 1;
        f4.mVertices.push_back(CVec2(-2,-2));
        f4.mVertices.push_back(CVec2(0,-2));
        f4.mVertices.push_back(CVec2(0,0));
        f4.mVertices.push_back(CVec2(-2,0));

        unsigned char r = rand() % 16;

        if (r & 0x01)
        {
            o->CreateFigure(f1);
        }

        if (r & 0x02)
        {
            o->CreateFigure(f2);
        }

        if (r & 0x04)
        {
        o->CreateFigure(f3);
        }

        if (r & 0x08)
        {
            o->CreateFigure(f4);
        }

        mTime = 0;
    }
}

}// namespace drash
