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

#include "ctestapp2.h"

#include "../cscene.h"

#include "../app/appeventprocessor.h"

namespace drash
{

bool CTestApp2::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->SetZ(300);

    SetProcessors();

    CSceneObjectGeometry g;
    g.mFigures.resize(1);
    g.mFigures[0].mVertices.push_back( CVec2( -300.0f, 5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( -300.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( 300.0f, -5.0f ) );
    g.mFigures[0].mVertices.push_back( CVec2( 300.0f, 5.0f ) );
    g.mFigures[0].mDepth = 5;
    CSceneObjectParams p;
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene().CreateObject<CSceneObject>(g, p);

    CSceneObjectGeometry player_geometry;
    player_geometry.mFigures.resize(1);
    player_geometry.mFigures[0].mVertices.push_back( CVec2( -2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2( 2, -5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2( 2, 5 ) );
    player_geometry.mFigures[0].mVertices.push_back( CVec2( -2, 5 ) );
    player_geometry.mFigures[0].mDepth = 1;
    CPlayerParams player;
    player.mPos.Set( 0, -20 );
    GetPlayersSystem().AddPlayer(player_geometry, player);

    CSceneObjectGeometry tg;
    tg.mFigures.resize(1);
    tg.mFigures[0].mVertices.push_back( CVec2( -10, -5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2( 10, -5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2( 10, 5 ) );
    tg.mFigures[0].mVertices.push_back( CVec2( -10, 5 ) );
    tg.mFigures[0].mFriction = 0.5;
    tg.mFigures[0].mMass = 1;
    tg.mFigures[0].mDepth = 1;
    CSceneObjectParams targetForFire;
    targetForFire.mPos.Set( -20, 0 );
    for (int i = 0 ; i < 10 ; i++)
    {
        GetScene().CreateObject<CSceneObject>(tg, targetForFire);
        targetForFire.mPos.Set( -20, 20 + i*20 );
    }

    return true;
}

void CTestApp2::SetProcessors()
{    
    auto t = GetTemplateSystem().CreateSceneObjectTemplate("lambda_test");
    t->mFigures.resize(1);
    t->mFigures[0].mVertices.push_back(CVec2(-10, -10));
    t->mFigures[0].mVertices.push_back(CVec2(10, -10));
    t->mFigures[0].mVertices.push_back(CVec2(0, 10));
    t->mFigures[0].mDepth = 3;



    GetEventSystem().SetProcessor("C-S-f", CAppEventProcessor(
    [this, t] ()
    {
        CSceneObjectParams p;
        p.mPos = GetCursorPos();
        GetDebugDrawSystem().ScreenSpaceToWorldSpace(p.mPos, -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
        GetTemplateSystem().CreateSceneObjectFromTemplate("lambda_test", p);
    }));



    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()// left mouse button pressed
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

}
