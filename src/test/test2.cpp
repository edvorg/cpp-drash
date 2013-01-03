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

#include "test2.h"

#include "../scene/scene.h"
#include "../app/appeventprocessor.h"
#include "../debugdrawsystem/camera.h"
#include "./misc/plane.h"
#include "../scene/sceneobject.h"
#include "../scene/player.h"
#include "../scene/figure.h"

namespace drash
{

namespace test
{

bool CTest2::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 0, 300));

    SetProcessors();

    CSceneObjectGeometry g;
    g.mFigures.resize(1);
    g.mFigures[0].mVertices.push_back( CVec2f( -300.0f, 5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2f( -300.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2f( 300.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2f( 300.0f, 5.0f ) );
    g.mFigures[0].mDepth = 5;
    CSceneObjectParams p;
    p.mPos.mY = -25;
    p.mDynamic = false;
    GetScene().CreateObject<CSceneObject>(g, p);

    CSceneObjectGeometry player_geometry;
    player_geometry.mFigures.resize(1);
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( -2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( 2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( 2, 5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( -2, 5 ) );
    player_geometry.mFigures[0].mDepth = 1;
    CPlayerParams player;
    player.mPos.Set(0, -20, 0);
    GetPlayersSystem().AddPlayer(player_geometry, player);

    CSceneObjectGeometry tg;
    tg.mFigures.resize(1);
    tg.mFigures[0].mVertices.push_back( CVec2f( -10, -5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2f( 10, -5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2f( 10, 5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2f( -10, 5 ) );
    tg.mFigures[0].mFriction = 0.5;
    tg.mFigures[0].mMass = 1;
    tg.mFigures[0].mDepth = 1;
    CSceneObjectParams targetForFire;
    targetForFire.mPos.Set(-20, 0, 0);
    for (int i = 0 ; i < 10 ; i++)
    {
        GetScene().CreateObject<CSceneObject>(tg, targetForFire);
        targetForFire.mPos.Set(-20, 20 + i*20, 0);
    }

    return true;
}

void CTest2::SetProcessors()
{    
    auto t = GetTemplateSystem().CreateSceneObjectTemplate("lambda_test");
    t->mFigures.resize(1);
    t->mFigures[0].mVertices.push_back(CVec2f(-10, -10));
    t->mFigures[0].mVertices.push_back(CVec2f(10, -10));
    t->mFigures[0].mVertices.push_back(CVec2f(0, 10));
    t->mFigures[0].mDepth = 3;



    GetEventSystem().SetProcessor("C-S-f", CAppEventProcessor(
    [this, t] ()
    {
        CPlane plane;
        plane.SetPoint(CVec3f(0));
        plane.SetNormal(CVec3f(0, 0, 1));

        CSceneObjectParams p;

        GetDebugDrawSystem().CastRay(GetCursorPos(), plane, p.mPos);

        GetTemplateSystem().CreateSceneObjectFromTemplate("lambda_test", p);
    }));



    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()// left mouse button pressed
    {
        // choose object here
		mSelectedObject = nullptr;
        CFigure *f = GetDebugDrawSystem().FindFigure(GetCursorPos());
        if (f != nullptr)
		{
			mSelectedObject = f->GetSceneObject();
		}
    },
    [this] ()// left mouse button is being pressed
    {
        // move object if choosen
        if (mSelectedObject != nullptr)
        {
              CPlane plane;
              plane.SetPoint(CVec3f(mSelectedObject->GetPos().Get()));
              plane.SetNormal(CVec3f(0, 0, 1));

              CVec3f pos;

              GetDebugDrawSystem().CastRay(GetCursorPos(), plane, pos);

              mSelectedObject->GetPos().Set(pos);
        }
    }));



    GetEventSystem().SetProcessor("LB C-d", CAppEventProcessor(
    [this] () // control-d pressed after LB released
    {
        // delete object if choosen
        if (mSelectedObject != nullptr)
        {
            GetScene().DestroyObject(mSelectedObject);
            mSelectedObject = nullptr;
        }
    }));



    GetEventSystem().SetProcessor("C-x C-c", CAppEventProcessor(
    [this] ()// key pressed. emacs like :)
    {
        this->Quit();
    }));
}

} // namespace test

} // namespace drash
