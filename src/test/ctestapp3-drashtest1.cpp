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
    sbp.mFigures.resize(1);
    sbp.mFigures[0].mLayers.Set( -20, 20 );
    sbp.mFigures[0].mRestitution = 0.0;
    sbp.mAngle = 0;
    sbp.mFigures[0].mVertices.push_back( CVec2( -300, -5 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 300, -5 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 300, 5 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( -300, 5 ) );
    GetScene()->CreateObject<CSolidBody>(sbp);

    sbp.mPos.Set( 0, 600 );
    GetScene()->CreateObject<CSolidBody>(sbp);

    sbp.mPos.Set( -300, 300 );
    sbp.mFigures[0].mVertices.clear();
    sbp.mFigures.resize(1);
    sbp.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    GetScene()->CreateObject<CSolidBody>(sbp);

    sbp.mPos.Set( 300, 300 );
    sbp.mFigures[0].mVertices.clear();
    sbp.mFigures.resize(1);
    sbp.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbp.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    GetScene()->CreateObject<CSolidBody>(sbp);

    CDrashBodyParams dbp;
    dbp.mPos.Set( 0, 100 );
    dbp.mAngle = - M_PI / 4;

    dbp.mFigures.resize(1);
    dbp.mFigures[0].mLayers.Set( -8, 8 );
    dbp.mFigures[0].mVertices.push_back( CVec2( -10, -10 ) );
    dbp.mFigures[0].mVertices.push_back( CVec2( 10, -10 ) );
    dbp.mFigures[0].mVertices.push_back( CVec2( 10, 10 ) );
    dbp.mFigures[0].mVertices.push_back( CVec2( -10, 10 ) );

    dbp.mDestroyDelay = 0.5;
    dbp.mDestroySpeed = 10.0f;

    GenDrashBodyParams( &dbp, 5, 0, 2 );

    CDrashBody *db = GetScene()->CreateObject<CDrashBody>(dbp);
    db->SetAngularVelocity(2);

    CSolidBodyParams pp;
    pp.mPos.Set( -200, 100 );
    pp.mFigures.resize(1);
    pp.mFigures[0].mMass = 3;
    GetScene()->CreateObject<CSolidBody>(pp)->SetLinearVelocity( CVec2( 200, 0 ) );

    CPlayerParams ppp;
    ppp.mPos.Set( 0, 10 );
    ppp.mFigures.resize(1);
    ppp.mFigures[0].mLayers.Set( -1, 1 );
    GetScene()->AddPlayer(ppp);

    GetCamera()->SetZoomTarget( 280, 1.0f );

    return true;
}

void CTestApp3::Release()
{
    CTestApp::Release();
}

void CTestApp3::Update()
{
    CTestApp::Update();

    if ( CPlayer *p = GetScene()->GetPlayer(0) )
    {
        GetCamera()->mPos.SetTarget( p->GetBody()->GetWorldCenter(), 1.0, AnimationBehaviorSingle );
    }
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

    printf( "level %i: (%i %i)\n",
            (int)_level,
            _params->mFigures[0].mLayers.GetMin(),
            _params->mFigures[0].mLayers.GetMax() );

    for ( int i = 0; i < 8; i++ )
    {
        _params->mChilds.push_back( CDrashBodyParams() );

        _params->mChilds[i].mDestroyDelay = _params->mDestroyDelay;
        _params->mChilds[i].mDestroySpeed = _params->mDestroySpeed;

        _params->mChilds[i].mFigures.resize(1);
        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( -_subset_size, -_subset_size ) );
        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( _subset_size, -_subset_size ) );
        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( _subset_size, _subset_size ) );
        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( -_subset_size, _subset_size ) );
    }

    _params->mChilds[0].mLocalPos.Set( -_subset_size, _subset_size );
    _params->mChilds[1].mLocalPos.Set( _subset_size, _subset_size );
    _params->mChilds[2].mLocalPos.Set( _subset_size, -_subset_size );
    _params->mChilds[3].mLocalPos.Set( -_subset_size, -_subset_size );
    _params->mChilds[4].mLocalPos.Set( -_subset_size, _subset_size );
    _params->mChilds[5].mLocalPos.Set( _subset_size, _subset_size );
    _params->mChilds[6].mLocalPos.Set( _subset_size, -_subset_size );
    _params->mChilds[7].mLocalPos.Set( -_subset_size, -_subset_size );

    int a = _params->mFigures[0].mLayers.GetMin();
    int b = ( _params->mFigures[0].mLayers.GetMax() + _params->mFigures[0].mLayers.GetMin() ) / 2;
    int c = _params->mFigures[0].mLayers.GetMax();
    _params->mChilds[0].mFigures[0].mLayers.Set( a, b-1 );
    _params->mChilds[1].mFigures[0].mLayers.Set( a, b-1 );
    _params->mChilds[2].mFigures[0].mLayers.Set( a, b-1 );
    _params->mChilds[3].mFigures[0].mLayers.Set( a, b-1 );
    _params->mChilds[4].mFigures[0].mLayers.Set( b, c );
    _params->mChilds[5].mFigures[0].mLayers.Set( b, c );
    _params->mChilds[6].mFigures[0].mLayers.Set( b, c );
    _params->mChilds[7].mFigures[0].mLayers.Set( b, c );

    for ( int i = 0; i < 8; i++ )
    {
        GenDrashBodyParams( &_params->mChilds[i], _subset_size / 2.0f, _level + 1, _levels );
    }
}

} // namespace drash

