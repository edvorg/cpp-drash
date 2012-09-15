#include "ctestapp2.h"

#include "../diag/ctimer.h"
#include "../cscene.h"
namespace drash{

CTestApp2::CTestApp2()
{
}

CTestApp2::~CTestApp2()
{
}

bool CTestApp2::Init( CScene *_scene, CCamera *_camera )
{
    if ( CTestApp::Init( _scene, _camera ) == false )
    {
        return false;
    }

    CSolidBodyParams p;
    p.mVertices.push_back( CVec2( -300.0f, 5.0f ) );
    p.mVertices.push_back( CVec2( -300.0f, -5.0f ) );
    p.mVertices.push_back( CVec2( 300.0f, -5.0f ) );
    p.mVertices.push_back( CVec2( 300.0f, 5.0f ) );
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene()->CreateObject<CSolidBody>(p);

    CPlayerParams player;
    player.mPos.Set( 0, -20 );
    player.mVertices.push_back( CVec2( -2, 0 ) );
    player.mVertices.push_back( CVec2( 2, 0 ) );
    player.mVertices.push_back( CVec2( 2, 10 ) );
    player.mVertices.push_back( CVec2( -2, 10 ) );
    GetScene()->AddPlayer(player);

    CSolidBodyParams targetForFire;
    targetForFire.mVertices.push_back( CVec2( -10, -5 ) );
    targetForFire.mVertices.push_back( CVec2( 10, -5 ) );
    targetForFire.mVertices.push_back( CVec2( 10, 5 ) );
    targetForFire.mVertices.push_back( CVec2( -10, 5 ) );
    targetForFire.mFriction = 0.5;
    targetForFire.mMass = 1;

    targetForFire.mPos.Set( -20, 0 );
    for (int i = 0 ; i < 10 ; i++)
    {
        GetScene()->CreateObject<CSolidBody>(targetForFire);
        targetForFire.mPos.Set( -20, 20 + i*20 );
    }

    return true;
}

void CTestApp2::Release()
{
    CTestApp::Release();
}

void CTestApp2::Update()
{
    CTestApp::Update();
}

void CTestApp2::Render()
{
    CTestApp::Render();
}
}
