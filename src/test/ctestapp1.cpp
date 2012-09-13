#include "ctestapp1.h"

#include "../cscene.h"
#include "../diag/ctimer.h"

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

    t.Reset(true);

    CSolidBodyParams p;
    p.mVertices.push_back( CVec2( -100.0f, 5.0f ) );
    p.mVertices.push_back( CVec2( -100.0f, -5.0f ) );
    p.mVertices.push_back( CVec2( 100.0f, -5.0f ) );
    p.mVertices.push_back( CVec2( 100.0f, 5.0f ) );
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
