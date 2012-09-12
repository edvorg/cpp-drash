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

    CSolidBodyParams sbp;
    sbp.mDynamic = false;
    sbp.mVertices.push_back( CVec2( -200, -5 ) );
    sbp.mVertices.push_back( CVec2( 200, -5 ) );
    sbp.mVertices.push_back( CVec2( 200, 5 ) );
    sbp.mVertices.push_back( CVec2( -200, 5 ) );

    CSolidBody* sb = GetScene()->CreateObject<CSolidBody>(sbp);

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

