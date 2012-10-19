#include "ctestapp2.h"

#include "../diag/ctimer.h"
#include "../cscene.h"

namespace drash
{

bool CTestApp2::Init()
{
    if ( CTestApp::Init() == false )
    {
        return false;
    }

    CSceneObjectParams p;
    p.mFigures.resize(1);
    p.mFigures[0].mVertices.push_back( CVec2( -300.0f, 5.0f ) );
    p.mFigures[0].mVertices.push_back( CVec2( -300.0f, -5.0f ) );
    p.mFigures[0].mVertices.push_back( CVec2( 300.0f, -5.0f ) );
    p.mFigures[0].mVertices.push_back( CVec2( 300.0f, 5.0f ) );
    p.mFigures[0].mLayers.Set(-500, 500);
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene().CreateObject<CSceneObject>(p);

    CPlayerParams player;
    player.mPos.Set( 0, -20 );
    player.mFigures.resize(1);
    player.mFigures[0].mVertices.push_back( CVec2( -2, -5 ) );
    player.mFigures[0].mVertices.push_back( CVec2( 2, -5 ) );
    player.mFigures[0].mVertices.push_back( CVec2( 2, 5 ) );
    player.mFigures[0].mVertices.push_back( CVec2( -2, 5 ) );
    player.mFigures[0].mLayers.Set(-100, 100);
    GetScene().AddPlayer(player);

    CSceneObjectParams targetForFire;
    targetForFire.mFigures.resize(1);
    targetForFire.mFigures[0].mVertices.push_back( CVec2( -10, -5 ) );
    targetForFire.mFigures[0].mVertices.push_back( CVec2( 10, -5 ) );
    targetForFire.mFigures[0].mVertices.push_back( CVec2( 10, 5 ) );
    targetForFire.mFigures[0].mVertices.push_back( CVec2( -10, 5 ) );
    targetForFire.mFigures[0].mFriction = 0.5;
    targetForFire.mFigures[0].mMass = 1;
    targetForFire.mFigures[0].mLayers.Set(-100, 100);

    targetForFire.mPos.Set( -20, 0 );
    for (int i = 0 ; i < 10 ; i++)
    {
        GetScene().CreateObject<CSceneObject>(targetForFire);
        targetForFire.mPos.Set( -20, 20 + i*20 );
    }

    return true;
}

}
