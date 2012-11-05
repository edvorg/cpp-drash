#include "cscene.h"
#include "diag/clogger.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <subsystem/subsystem.h>

namespace drash
{

CScene::CScene(void):
    mWorld( b2Vec2( 0, 0 ) ),
    mInitialized(false),
    mCountPlayers(0),
    mObjectsCount(0),
    mSubsystemsCount(0)
{
}

CScene::~CScene(void)
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::~CScene(): "
                  "Release() called automatically from destructor" );
        Release();
    }
}

bool CScene::Init( const CSceneParams &_params )
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::Init(): "
                  "already initialized" );
        return false;
    }

    mWorld.SetContactFilter(this);
    mWorld.SetContactListener(this);
    mWorld.SetAllowSleeping(true);
    mWorld.SetContinuousPhysics(false);
    mWorld.SetGravity( _params.mGravity );

    mInitialized = true;
    return true;
}

void CScene::Release(void)
{
    if ( mInitialized == false )
    {
        LOG_WARN( "CScene::Release(): "
                  "already released" );
        return;
    }

    if ( mObjectsCount != 0 )
    {
        LOG_WARN( "CScene::Release(): "<<
                  mObjectsCount<<
                  " object(s) haven't been destroyed. Autorelease" );

        while( mObjectsCount != 0 )
        {
            DestroyObject<CSceneObject>( mObjects[0] );
        }
    }

    mInitialized = false;
}

const CScene::CSceneObjectPtr *CScene::GetObjects()
{
    return mObjects;
}

unsigned int CScene::EnumObjects() const
{
    return mObjectsCount;
}

void CScene::Step( double _dt )
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): "
                 "this is not initialized" );
        return;
    }

    for ( unsigned int i = 0 ; i < mObjectsCount ; i++ )
    {
        mObjects[i]->Step(_dt);
    }

    for ( unsigned int i = 0; i < mObjectsCount; )
    {
        if ( mObjects[i]->IsDead() )
        {
            DestroyObject<CSceneObject>( mObjects[i] );
        }
        else
        {
            i++;
        }
    }

    BoomNow();

    mWorld.Step( _dt, mVelocityIterations, mPositionIterations );
}

bool CScene::ShouldCollide( b2Fixture *fixtureA, b2Fixture *fixtureB )
{
    CInterval *i1 = reinterpret_cast<CInterval*>( fixtureA->GetUserData() );
    CInterval *i2 = reinterpret_cast<CInterval*>( fixtureB->GetUserData() );

    if ( i1 && i2 )
    {
        return i1->IsIntersects(*i2);
    }

    return false;
}

void CScene::BeginContact( b2Contact * _contact )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of objects. Skipping" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactBegin(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactBegin(c);
}

void CScene::PreSolve( b2Contact *_contact, const b2Manifold *_oldManifold )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of objects. Skipping" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactPreSolve(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactPreSolve(c);
}

void CScene::EndContact( b2Contact *_contact )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                         _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                         _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "one of objects has NULL user data pointer (CSceneObject)" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i = 0; i < c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactEnd(c);

    for ( unsigned int i = 0; i < c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactEnd(c);
}

void CScene::Draw( const CCamera &_camera )
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): "
                 "this is not initialized" );
        return;
    }

    for ( b2Body *b = mWorld.GetBodyList(); b != NULL; b = b->GetNext() )
    {
        auto o = reinterpret_cast<CSceneObject*>( b->GetUserData() );
        if (o)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            //glRotatef( 30, 0, 1, 0 );
            glTranslatef( -_camera.mPos.Get().x, -_camera.mPos.Get().y, -_camera.m_ZoomMax + _camera.GetZoom() );
            glRotatef(15, 1, 0, 0);
            glTranslatef( o->GetBody()->GetWorldCenter().x, o->GetBody()->GetWorldCenter().y, 0 );
            glRotatef( 180.0f / M_PI * o->GetBody()->GetAngle(), 0, 0, 1 );

            o->DrawDebug();
        }
    }
}

void CScene::AddRequestBoom( const CBoomParams _boom )
{
    mListBooms.push_back(_boom);
}

void CScene::BoomNow()
{
    for ( unsigned int i = 0; i < mObjectsCount; i++ )
    {
        for ( auto it = mListBooms.begin() ; it != mListBooms.end() ; it++ )
        {
            mObjects[i]->OnBoom(*it);
        }
    }

    mListBooms.clear();
}

void CScene::ConnectSubsystem(CSubsystem *_subsystem)
{
    if (_subsystem == NULL)
    {
        return;
    }

    if (mSubsystemsCount >= mMaxSubsystemsCount)
    {
        LOG_ERR("CScene::AddSubsystem(): Unable to connect with subsystem. Connection count is maximal");
        return;
    }

    for (unsigned int i=0; i<mSubsystemsCount; i++)
    {
        if (mSubsystems[i] == _subsystem)
        {
            LOG_WARN("CScene::AddSubsystem(): subsystem already connected");
            return;
        }
    }

    mSubsystems[mSubsystemsCount++] = _subsystem;
}

void CScene::DisconnectSubsystem(CSubsystem *_subsystem)
{
    if (_subsystem == NULL)
    {
        return;
    }

    for (unsigned int i = 0; i < mSubsystemsCount; i++)
    {
        if (mSubsystems[i] == _subsystem)
        {
            if (mSubsystemsCount-- != 1)
            {
                mSubsystems[i] = mSubsystems[mSubsystemsCount];
                mSubsystems[mSubsystemsCount] = NULL;
            }
            else
            {
                mSubsystems[0] = NULL;
            }
            return;
        }
    }

    LOG_WARN("CScene::RemSubsystem(): subsystem is not connected");
}

const CScene::CSubsystemPtr *CScene::GetSubsystems()
{
    return mSubsystems;
}

unsigned int CScene::EnumSubsystems() const
{
    return mSubsystemsCount;
}

} // namespace drash
