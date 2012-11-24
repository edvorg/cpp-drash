#include "cscene.h"
#include "diag/clogger.h"
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
            DestroyObject(mObjects[0]);
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
            DestroyObjectImpl(mObjects[i]);
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

void CScene::DestroyObject(CSceneObject *_obj)
{
    DRASH_ASSERT(_obj != nullptr &&
                 "CScene::DestroyObject(): wrong pointer");
    DRASH_ASSERT( mObjects[_obj->mInternalId] == _obj &&
                  "CScene::DestroyObject(): something wrong with objects creation logic" );

    if (mLocked == false && mWorld.IsLocked() == false)
    {
        DestroyObjectImpl(_obj);
    }
    else
    {
        _obj->mDead = true;
    }
}

void CScene::DestroyObjects(void)
{
    for (unsigned int i = 0 ; i < this->EnumObjects() ; i++)
    {
        DestroyObject(this->GetObjects()[i]);
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

void CScene::DestroyObjectImpl(CSceneObject *_obj)
{
    b2Body* body = _obj->mBody;
    _obj->mBody->SetActive(false);
    _obj->mBody->SetUserData(NULL);

    mObjectsCount--;
    mObjects[_obj->mInternalId] = mObjects[mObjectsCount];
    mObjects[mObjectsCount] = NULL;

    if ( mObjects[_obj->mInternalId] != NULL )
    {
        mObjects[_obj->mInternalId]->mInternalId = _obj->mInternalId;
    }

    _obj->Release();
    delete _obj;

    mWorld.DestroyBody(body);
}

} // namespace drash
