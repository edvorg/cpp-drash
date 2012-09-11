#include "ctestapp1.h"

#include "../cscene.h"
#include "../ctimer.h"

namespace drash
{

CTestApp1::CTestApp1()
{
}

CTestApp1::~CTestApp1()
{
}

CTimer t;

bool CTestApp1::Init( CScene *_scene, CCamera *_camera )
{
    if ( CTestApp::Init( _scene, _camera ) == false )
    {
        return false;
    }

    // TODO: realize this
    // GetCamera()->SetZoomMax(50);

    t.Reset(true);

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

    return true;
}

void CTestApp1::Release()
{
    CTestApp::Release();
}

void CTestApp1::Update()
{
    CTestApp::Update();

    t.Tick();
    if ( t.GetFullTime() > 1 )
    {
        CDrashBodyParams p;
        p.mPos.RandY(100, 200, 15);
        p.mPos.RandX(-50, 50, 15);
        GetScene()->CreateObject<CDrashBody>(p);
        t.Reset(true);
    }
}

void CTestApp1::Render()
{
    CTestApp::Render();
}

} // namespace drash
