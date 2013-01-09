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

#include "test2.h"

#include "../scene/scene.h"
#include "../app/appeventprocessor.h"
#include "../greng/camera.h"
#include "../misc/plane.h"
#include "../scene/sceneobject.h"
#include "../players/player.h"
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

    greng::CCameraParams cp;
    cp.mPos.Set(0, 0, 300);
    mCamera = GetCameraManager().CreateCamera(cp);

    if (mCamera == nullptr)
    {
        return false;
    }

    GetDebugRenderer().SetCamera(mCamera);

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
    GetScene().CreateObject(g, p);

    CSceneObjectGeometry player_geometry;
    player_geometry.mFigures.resize(1);
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( -2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( 2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( 2, 5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2f( -2, 5 ) );
    player_geometry.mFigures[0].mDepth = 1;

    CPlayerParams player;
    player.mSceneObjectParams.mPos.Set(0, -20, 0);

    GetPlayersSystem().CreatePlayer(player_geometry, player);

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
        GetScene().CreateObject(tg, targetForFire);
        targetForFire.mPos.Set(-20, 20 + i*20, 0);
    }

    mLight1.mPosition.Set(0, 30, 0);

    GetDebugRenderer().SetLight(&mLight1);

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

        mCamera->CastRay(GetCursorPos(), plane, p.mPos);

        GetTemplateSystem().CreateSceneObjectFromTemplate("lambda_test", p);
    }));



    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()// left mouse button pressed
    {
        // choose object here
		mSelectedObject = nullptr;
        CFigure *f = GetDebugRenderer().FindFigure(mCamera, GetCursorPos());
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
              plane.SetPoint(mSelectedObject->GetPos());
              plane.SetNormal(CVec3f(0, 0, 1));

              CVec3f pos;

              mCamera->CastRay(GetCursorPos(), plane, pos);

              mSelectedObject->SetPos(pos);
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
