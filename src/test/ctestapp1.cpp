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

#include "ctestapp1.h"

#include "../cscene.h"

namespace drash
{

bool CTestApp1::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    CSceneObjectGeometry g;
    g.mFigures.resize(1);
    g.mFigures[0].mDepth = 5;
    g.mFigures[0].mVertices.push_back( CVec2( -100.0f, 5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( -100.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( 100.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( 100.0f, 5.0f ) );
    CSceneObjectParams p;
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene().CreateObject<CSceneObject>(g, p);

    return true;
}

void CTestApp1::Step(double _dt)
{
    CApp::Step(_dt);

    mTime += _dt;

    if (mTime > 1.0)
    {
        CSceneObjectGeometry g;
        g.mFigures.resize(1);
        g.mFigures[0].mDepth = 5;
        CDrashBodyParams p;
        p.mPos.RandY(100, 200, 15);
        p.mPos.RandX(-50, 50, 15);
        GetScene().CreateObject<CSceneObject>(g, p);
        mTime = 0;
    }
}

} // namespace drash
