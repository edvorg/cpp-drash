#include "ctestapp3-drashtest1.h"

#include "../cscene.h"
#include "../sceneobjects.h"

namespace drash
{

void GenDrashBodyParams( CDrashBodyParams* _params , float _subset_size, unsigned int _level, unsigned int _levels );

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
    sbp.mRestitution = 0.0;
    sbp.mAngle = -0.1;
    sbp.mVertices.push_back( CVec2( -300, -5 ) );
    sbp.mVertices.push_back( CVec2( 300, -5 ) );
    sbp.mVertices.push_back( CVec2( 300, 5 ) );
    sbp.mVertices.push_back( CVec2( -300, 5 ) );
    GetScene()->CreateObject<CSolidBody>(sbp);

    CDrashBodyParams dbp;
    dbp.mPos.Set(0, 100);
    dbp.mAngle = - M_PI / 4;

    dbp.mVertices.push_back( CVec2( -10, -10 ) );
    dbp.mVertices.push_back( CVec2( 10, -10 ) );
    dbp.mVertices.push_back( CVec2( 10, 10 ) );
    dbp.mVertices.push_back( CVec2( -10, 10 ) );

    dbp.mDestroyDelay = 0.5;

    GenDrashBodyParams( &dbp, 5, 0, 4 );

    CDrashBody *db = GetScene()->CreateObject<CDrashBody>(dbp);
    db->GetBody()->SetAngularVelocity(2);

    CSolidBodyParams pp;
    pp.mPos.Set(-200, 100);
    pp.mMass = 3;
    GetScene()->CreateObject<CSolidBody>(pp)->GetBody()->SetLinearVelocity( CVec2(200, 0) );

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

void GenDrashBodyParams( CDrashBodyParams* _params, float _subset_size, unsigned int _level, unsigned int _levels )
{
    if ( _level > _levels )
    {
        return;
    }

    for ( int i = 0; i < 4; i++ )
    {
        _params->mChilds.push_back( CDrashBodyParams() );

        _params->mChilds[i].mDestroyDelay = 0.5;

        _params->mChilds[i].mVertices.push_back( CVec2( -_subset_size, -_subset_size ) );
        _params->mChilds[i].mVertices.push_back( CVec2( _subset_size, -_subset_size ) );
        _params->mChilds[i].mVertices.push_back( CVec2( _subset_size, _subset_size ) );
        _params->mChilds[i].mVertices.push_back( CVec2( -_subset_size, _subset_size ) );
    }

    _params->mChilds[0].mLocalPos.Set( -_subset_size, _subset_size );
    _params->mChilds[1].mLocalPos.Set( _subset_size, _subset_size );
    _params->mChilds[2].mLocalPos.Set( _subset_size, -_subset_size );
    _params->mChilds[3].mLocalPos.Set( -_subset_size, -_subset_size );

    for ( int i = 0; i < 4; i++ )
    {
        GenDrashBodyParams( &_params->mChilds[i], _subset_size / 2.0f, _level + 1, _levels );
    }
}

} // namespace drash

