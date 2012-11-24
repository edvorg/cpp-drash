#include "ctestapp2.h"

#include "../cscene.h"

namespace drash
{

bool CTestApp2::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

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

}
