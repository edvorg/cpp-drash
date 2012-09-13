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
    sbp.mRestitution = 0.5;
    sbp.mAngle = 0.1;
    sbp.mVertices.push_back( CVec2( -200, -5 ) );
    sbp.mVertices.push_back( CVec2( 200, -5 ) );
    sbp.mVertices.push_back( CVec2( 200, 5 ) );
    sbp.mVertices.push_back( CVec2( -200, 5 ) );

    CSolidBody* sb = GetScene()->CreateObject<CSolidBody>(sbp);

    CDrashBodyParams dbp;
    dbp.mDynamic = true;
    dbp.mPos.Set(0, 100);

    dbp.mVertices.push_back( CVec2( -10, -10 ) );
    dbp.mVertices.push_back( CVec2( 10, -10 ) );
    dbp.mVertices.push_back( CVec2( 10, 10 ) );
    dbp.mVertices.push_back( CVec2( -10, 10 ) );

    for ( int i = 0; i < 4; i++ )
    {
        dbp.mChilds.push_back( CDrashBodyParams() );
        dbp.mChilds[i].mVertices.push_back( CVec2( -5, -5 ) );
        dbp.mChilds[i].mVertices.push_back( CVec2( 5, -5 ) );
        dbp.mChilds[i].mVertices.push_back( CVec2( 5, 5 ) );
        dbp.mChilds[i].mVertices.push_back( CVec2( -5, 5 ) );

        for ( int j = 0; j < 4; j++ )
        {
            dbp.mChilds[i].mChilds.push_back( CDrashBodyParams() );
            dbp.mChilds[i].mChilds[j].mVertices.push_back( CVec2( -2.5, -2.5 ) );
            dbp.mChilds[i].mChilds[j].mVertices.push_back( CVec2( 2.5, -2.5 ) );
            dbp.mChilds[i].mChilds[j].mVertices.push_back( CVec2( 2.5, 2.5 ) );
            dbp.mChilds[i].mChilds[j].mVertices.push_back( CVec2( -2.5, 2.5 ) );
        }
    }

    dbp.mChilds[0].mLocalPos.Set(-5, 5);
    dbp.mChilds[0].mChilds[0].mLocalPos.Set(-2.5, 2.5);
    dbp.mChilds[0].mChilds[1].mLocalPos.Set(2.5, 2.5);
    dbp.mChilds[0].mChilds[2].mLocalPos.Set(2.5, -2.5);
    dbp.mChilds[0].mChilds[3].mLocalPos.Set(-2.5, -2.5);

    dbp.mChilds[1].mLocalPos.Set(5, 5);
    dbp.mChilds[1].mChilds[0].mLocalPos.Set(-2.5, 2.5);
    dbp.mChilds[1].mChilds[1].mLocalPos.Set(2.5, 2.5);
    dbp.mChilds[1].mChilds[2].mLocalPos.Set(2.5, -2.5);
    dbp.mChilds[1].mChilds[3].mLocalPos.Set(-2.5, -2.5);

    dbp.mChilds[2].mLocalPos.Set(5, -5);
    dbp.mChilds[2].mChilds[0].mLocalPos.Set(-2.5, 2.5);
    dbp.mChilds[2].mChilds[1].mLocalPos.Set(2.5, 2.5);
    dbp.mChilds[2].mChilds[2].mLocalPos.Set(2.5, -2.5);
    dbp.mChilds[2].mChilds[3].mLocalPos.Set(-2.5, -2.5);

    dbp.mChilds[3].mLocalPos.Set(-5, -5);
    dbp.mChilds[3].mChilds[0].mLocalPos.Set(-2.5, 2.5);
    dbp.mChilds[3].mChilds[1].mLocalPos.Set(2.5, 2.5);
    dbp.mChilds[3].mChilds[2].mLocalPos.Set(2.5, -2.5);
    dbp.mChilds[3].mChilds[3].mLocalPos.Set(-2.5, -2.5);


    CDrashBody *db = GetScene()->CreateObject<CDrashBody>(dbp);

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

