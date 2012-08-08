#include "ctestapp2.h"

#include "ctimer.h"
#include "cscene.h"
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

    GetCamera()->SetZoomMax(50);

    const unsigned int vc = 4;
    const CVec2 v[vc] =
    {
        CVec2( -100.0f, 5.0f ),
        CVec2( -100.0f, -5.0f ),
        CVec2( 100.0f, -5.0f ),
        CVec2( 100.0f, 5.0f )
    };
    CSolidBodyParams p;
    p.mVertices = v;
    p.mVerticesCount = vc;
    p.mPos.y = -25;
    p.mDynamic = false;
    GetScene()->CreateObject<CSolidBody>(p);
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
    this->GetCamera()->SetZoomTarget(3.4f);
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