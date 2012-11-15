#include "cscene.h"
#include "diag/clogger.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <subsystem/subsystem.h>

namespace drash
{

CScene::CScene(void):
    mInitialized(false),
    mLocked(false),
    mWorld( b2Vec2( 0, 0 ) ),
    mObserver(),
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

    mWorld.SetContactFilter(&mObserver);
    mWorld.SetContactListener(&mObserver);
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

void CScene::Step( double _dt )
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): "
                 "this is not initialized" );
        return;
    }

    mLocked = true;

    for ( unsigned int i = 0; i < mObjectsCount; )
    {
        if ( mObjects[i]->mDead )
        {
            DestroyObjectImpl<CSceneObject>( mObjects[i] );
        }
        else
        {
            mObjects[i]->Step(_dt);
            i++;
        }
    }

    mLocked = false;

    mWorld.Step( _dt, mVelocityIterations, mPositionIterations );
}

const CScene::ObjectsT &CScene::GetObjects(void) const
{
    return mObjects;
}

unsigned int CScene::EnumObjects(void) const
{
    return mObjectsCount;
}

void CScene::DestroyObjects(void)
{
    for (unsigned int i = 0 ; i < this->EnumObjects() ; i++)
    {
        DestroyObject(this->GetObjects()[i]);
    }
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
            glTranslatef( o->GetBody()->GetWorldCenter().x, o->GetBody()->GetWorldCenter().y, 0 );
            glRotatef( 180.0f / M_PI * o->GetBody()->GetAngle(), 0, 0, 1 );

            o->DrawDebug();
        }
    }
}

void CScene::ConnectSubsystem(CSubsystem *_subsystem)
{
    if (_subsystem == NULL)
    {
        return;
    }

    if (mSubsystemsCount >= mSubsystemsCountLimit)
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

const CScene::SystemsT &CScene::GetSubsystems(void) const
{
    return mSubsystems;
}

unsigned int CScene::EnumSubsystems(void) const
{
    return mSubsystemsCount;
}

} // namespace drash
