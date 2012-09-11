#include "ctestapp3-drashtest1.h"

#include "../cscene.h"
#include "../sceneobjects.h"

namespace drash
{

CTestApp3::CTestApp3()
{
}

CTestApp3::~CTestApp3()
{
}

bool CTestApp3::Init( CScene *_scene, CCamera *_camera )
{
    if ( CTestApp::Init( _scene, _camera ) == false )
    {
        return false;
    }

    const int sbvc = 4;
    CVec2 sbv[sbvc] =
    {
        CVec2( -200, -5 ),
        CVec2( 200, -5 ),
        CVec2( 200, 5 ),
        CVec2( -200, 5 )
    };

    CSolidBodyParams sbp;
    sbp.mDynamic = false;
    sbp.mVertices = sbv;
    sbp.mVerticesCount = sbvc;

    CSolidBody* sb = GetScene()->CreateObject<CSolidBody>(sbp);

    sb->mPos.Set( CVec2( 0, -20 ) );
    sb->mPos.SetTarget( CVec2( 0, -100 ), 1.0, AnimationBehaviorBounce );

    return true;
}

void CTestApp3::Release()
{
    CTestApp::Release();
}

void CTestApp3::Update()
{
    CTestApp::Update();
}

void CTestApp3::Render()
{
    CTestApp::Render();
}

} // namespace drash

