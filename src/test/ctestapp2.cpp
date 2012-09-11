#include "ctestapp2.h"

#include "../ctimer.h"
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

    // TODO: realize this
    // GetCamera()->SetZoomMax(50);

    const unsigned int vc = 4;
    const CVec2 v[vc] =
    {
        CVec2( -300.0f, 5.0f ),
        CVec2( -300.0f, -5.0f ),
        CVec2( 300.0f, -5.0f ),
        CVec2( 300.0f, 5.0f )
    };
    CSolidBodyParams p;
    p.mVertices = v;
    p.mVerticesCount = vc;
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene()->CreateObject<CSolidBody>(p);//->SetTargetAngle(M_PI / 6.0f);
    CPlayerParams player;
    const CVec2 pV[4] = {
        CVec2( -2, 0 ),
        CVec2( 2, 0 ),
        CVec2( 2, 10 ),
        CVec2( -2, 10 )
    };
    player.mPos.Set(0,-20);
    player.mVertices = pV;
    player.mVerticesCount = 4;
    GetScene()->AddPlayer(player);

    const CVec2 tV[4] = {
        CVec2( -10, 0 ),
        CVec2( 10, 0 ),
        CVec2( 10, 10 ),
        CVec2( -10, 10 )
    };
    CSolidBodyParams targetForFire;
    targetForFire.mVertices = tV;
    targetForFire.mVerticesCount = 4;
    targetForFire.mFriction = 5;
    targetForFire.mMass = 150;

    targetForFire.mPos.Set(-20,0);
    for (int i = 0 ; i < 10 ; i++){
        GetScene()->CreateObject<CSolidBody>(targetForFire);
        targetForFire.mPos.Set(-20,i*10);
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
