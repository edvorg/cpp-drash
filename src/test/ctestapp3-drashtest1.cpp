#include "ctestapp3-drashtest1.h"

#include "../cscene.h"
#include "../sceneobjects.h"

namespace drash
{

void GenDrashBodyParams( CDrashBodyParams* _params , float _subset_size, unsigned int _level, unsigned int _levels );

bool CTestApp3::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    CSceneObjectGeometry sbg;
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mDepth = 40;
    sbg.mFigures[0].mRestitution = 0.0;
    sbg.mFigures[0].mVertices.push_back( CVec2( -300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 300, 5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -300, 5 ) );
    CSceneObjectParams sbp;
    sbp.mDynamic = false;
    sbp.mAngle = 0;
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbp.mPos.Set( 0, 600 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set( -300, 300 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set( 300, 300 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    CSceneObjectGeometry dbg;
    dbg.mFigures.resize(1);
    dbg.mFigures[0].mDepth = 16;
    dbg.mFigures[0].mVertices.push_back( CVec2( -10, -10 ) );
    dbg.mFigures[0].mVertices.push_back( CVec2( 10, -10 ) );
    dbg.mFigures[0].mVertices.push_back( CVec2( 10, 10 ) );
    dbg.mFigures[0].mVertices.push_back( CVec2( -10, 10 ) );
    CDrashBodyParams dbp;
    dbp.mPos.Set( 0, 100 );
    dbp.mAngle = - M_PI / 4;
    dbp.mDestroyDelay = 0.5;
    dbp.mDestroySpeed = 7.0f;

    GenDrashBodyParams( &dbp, 5, 0, 2 );

    CDrashBody *db = GetScene().CreateObject<CDrashBody>(dbg, dbp);
    db->SetAngularVelocity(2);
    db->SetLinearVelocity( CVec2(0, -20) );

    CSceneObjectGeometry pg;
    pg.mFigures.resize(1);
    pg.mFigures[0].mMass = 3;
    CSceneObjectParams pp;
    pp.mPos.Set( -200, 100 );
    GetScene().CreateObject<CSceneObject>(pg, pp)->SetLinearVelocity( CVec2( 200, 0 ) );

    CSceneObjectGeometry ppg;
    ppg.mFigures.resize(1);
    ppg.mFigures[0].mDepth = 2;
    CPlayerParams ppp;
    ppp.mPos.Set( 0, 10 );
    GetPlayersSystem().AddPlayer(ppg, ppp);

    CSceneObjectGeometry platform_geometry;
    platform_geometry.mFigures.resize(1);
    platform_geometry.mFigures[0].mDepth = 8;
    platform_geometry.mFigures[0].mFriction = 1.0;
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( -100, -5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( 100, -5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( 100, 5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( -100, 5 ) );
    CSceneObject::ParamsT platform_params;
    platform_params.mPos.Set( 0, 50 );
    platform_params.mAngle = -M_PI / 18.0;
    platform_params.mDynamic = false;

    CSceneObject *platform = GetScene().CreateObject<CSceneObject>(platform_geometry, platform_params);
    platform->SetPos( CVec2( -100, 50 ) );
    platform->SetPosTarget( CVec2( 100, 50 ), 10, AnimationBehaviorBounce );
    platform->SetAngleTarget( M_PI / 18.0, 10, AnimationBehaviorBounce );

    GetDebugDrawSystem().GetActiveCam()->SetZTarget( 280, 1.0f, AnimationBehaviorSingle );

    return true;
}

void CTestApp3::Step(double _dt)
{
    CApp::Step(_dt);

    for (CAppEvent e=PopEvent(); e.GetType()!=EventUnknown; e=PopEvent())
    {
        if (e.GetType() == EventKeyboard)
        {
            switch (e.GetKey())
            {
            case EventKeyW:
                GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveDeep ), 0 );
                break;
            case EventKeyA:
                GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveLeft ), 0 );
                break;
            case EventKeyS:
                GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveOut ), 0 );
                break;
            case EventKeyD:
                GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveRight ), 0 );
                break;
            case EventKeySpace:
                GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionJump ), 0 );
                break;
            default:
                break;
            }
        }
    }

    if (GetPlayersSystem().EnumPlayers())
    {
        CPlayer *p = GetPlayersSystem().GetPlayers()[0];
        GetDebugDrawSystem().GetActiveCam()->SetPosTarget( p->GetBody()->GetWorldCenter(), 1.0, AnimationBehaviorSingle );
    }
}

void GenDrashBodyParams( CDrashBodyParams* _params, float _subset_size, unsigned int _level, unsigned int _levels )
{
//    if ( _level > _levels )
//    {
//        return;
//    }

//    for ( int i = 0; i < 8; i++ )
//    {
//        _params->mChilds.push_back( CDrashBodyParams() );

//        _params->mChilds[i].mDestroyDelay = _params->mDestroyDelay;
//        _params->mChilds[i].mDestroySpeed = _params->mDestroySpeed;

//        _params->mChilds[i].mFigures.resize(1);
//        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( -_subset_size, -_subset_size ) );
//        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( _subset_size, -_subset_size ) );
//        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( _subset_size, _subset_size ) );
//        _params->mChilds[i].mFigures[0].mVertices.push_back( CVec2( -_subset_size, _subset_size ) );
//    }

//    _params->mChilds[0].mLocalPos.Set( -_subset_size, _subset_size );
//    _params->mChilds[1].mLocalPos.Set( _subset_size, _subset_size );
//    _params->mChilds[2].mLocalPos.Set( _subset_size, -_subset_size );
//    _params->mChilds[3].mLocalPos.Set( -_subset_size, -_subset_size );
//    _params->mChilds[4].mLocalPos.Set( -_subset_size, _subset_size );
//    _params->mChilds[5].mLocalPos.Set( _subset_size, _subset_size );
//    _params->mChilds[6].mLocalPos.Set( _subset_size, -_subset_size );
//    _params->mChilds[7].mLocalPos.Set( -_subset_size, -_subset_size );

//    int a = _params->mFigures[0].mLayers.GetMin();
//    int b = ( _params->mFigures[0].mLayers.GetMax() + _params->mFigures[0].mLayers.GetMin() ) / 2;
//    int c = _params->mFigures[0].mLayers.GetMax();
//    _params->mChilds[0].mFigures[0].mLayers.Set( a, b-1 );
//    _params->mChilds[1].mFigures[0].mLayers.Set( a, b-1 );
//    _params->mChilds[2].mFigures[0].mLayers.Set( a, b-1 );
//    _params->mChilds[3].mFigures[0].mLayers.Set( a, b-1 );
//    _params->mChilds[4].mFigures[0].mLayers.Set( b, c );
//    _params->mChilds[5].mFigures[0].mLayers.Set( b, c );
//    _params->mChilds[6].mFigures[0].mLayers.Set( b, c );
//    _params->mChilds[7].mFigures[0].mLayers.Set( b, c );

//    for ( int i = 0; i < 8; i++ )
//    {
//        GenDrashBodyParams( &_params->mChilds[i], _subset_size / 2.0f, _level + 1, _levels );
//    }
}

} // namespace drash

