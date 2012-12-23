// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
    mWorld.SetGravity(CVec2ToB2Vec2(_params.mGravity));

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

    while (mWorld.GetJointCount())
    {
        delete reinterpret_cast<CJoint*>(mWorld.GetJointList()->GetUserData());
        mWorld.GetJointList()->SetUserData(nullptr);
        mWorld.DestroyJoint(mWorld.GetJointList());
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

CJoint *CScene::CreateJoint(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor)
{
    b2WeldJointDef jdef;
    jdef.Initialize(_obj1->mBody, _obj2->mBody, CVec2ToB2Vec2(_anchor.Vec2()));

    CJoint *res = new CJoint;
    res->mJoint = mWorld.CreateJoint(&jdef);;
    res->mJoint->SetUserData(res);
    return res;
}

void CScene::DestroyJoint(CJoint *_joint)
{
    for (auto j=mWorld.GetJointList(); j!=nullptr; j=j->GetNext())
    {
        if (j->GetUserData() == _joint)
        {
            delete reinterpret_cast<CJoint*>(j->GetUserData());
            mWorld.DestroyJoint(j);
            return;
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

void CScene::SetGravity(const CVec2f &_g)
{
    mWorld.SetGravity(CVec2ToB2Vec2(_g));
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
