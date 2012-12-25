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

#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "physobserver.h"
#include "misc/vec2.h"
#include "misc/vec3.h"

namespace drash
{

class CSceneObject;
class CJoint;

class CSceneParams
{
public:
    CVec2f mGravity;
};

class CSubsystem;

class CScene
{
public:    
    // **************************************************
    // * static scene configuration *********************

    static const int mVelocityIterations = 5;
    static const int mPositionIterations = 2;
    static const unsigned int mObjectsCountLimit = 10000;
    static const unsigned int mSubsystemsCountLimit = 5;

    // **************************************************
    // * main routines **********************************

    CScene(void);
    virtual ~CScene(void);

    bool Init( const CSceneParams& _params );
    void Release(void);

    /// must be called once in update cycle
    /// dt - seconds
    void Step( double _dt );

    // **************************************************
    // * working with objects ***************************

    /// T must extend CSceneObject class
    /// Method excepts, that T has ParamsT and GeometryT typedefs
    template < typename T >
    T* CreateObject( const typename T::GeometryT &_geometry, const typename T::ParamsT& _params );

    void DestroyObject(CSceneObject *_obj);

    inline CSceneObject * const * GetObjects(void) const;

    /// returns total objects count
    inline unsigned int EnumObjects(void) const;

    /// destroyes all objects
    void DestroyObjects(void);

    // **************************************************
    // * working with joints ****************************

    CJoint *CreateJoint(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor);
    void DestroyJoint(CJoint *_joint);

    // **************************************************
    // * working with subsystems ************************

    /// as we made a connection, _subsystem gets a pointer to our CScene instance
    /// and starts interaction with it
    void ConnectSubsystem(CSubsystem *_subsystem);

    /// as we made disconnect _subsystem, we lose pointer to this CScene instance
    void DisconnectSubsystem(CSubsystem *_subsystem);

    inline CSubsystem * const * GetSubsystems(void) const;

    /// returns total connected subsystems count
    inline unsigned int EnumSubsystems(void) const;

    void SetGravity(const CVec2f &_g);

protected:
private:
    void DestroyObjectImpl(CSceneObject *_obj);

    bool mInitialized = false;
    bool mLocked = false;

    b2World mWorld;
    CPhysObserver mObserver;

    CSceneObject *mObjects[mObjectsCountLimit];
    unsigned int mObjectsCount = 0;

    CSubsystem *mSubsystems[mSubsystemsCountLimit];
    unsigned int mSubsystemsCount = 0;
};

template < typename T >
T* CScene::CreateObject(const typename T::GeometryT &_geometry, const typename T::ParamsT& _params)
{
    if (mObjectsCount == mObjectsCountLimit)
	{
        LOG_ERR("CScene::CreateObject(): Achieved maximum Amount of Objects in scene");
        return nullptr;
    }

    if (mWorld.IsLocked())
    {
        LOG_ERR("CScene::CreateObject(): world is locked now");
        return nullptr;
    }

    b2BodyDef bdef;
    b2Body *b = mWorld.CreateBody(&bdef);

    if (b == nullptr)
    {
        LOG_ERR("CScene::CreateObject(): something wrong with box2d");
        return nullptr;
    }

    T* res = new T();
    res->mBody = b;
    res->mBody->SetUserData(res);
    res->mScene = this;
    res->mInternalId = mObjectsCount;

    if (res->Init(_geometry, _params) == false)
    {
        LOG_ERR("CScene::CreateObject(): object init failed");
        mWorld.DestroyBody(b);
        delete res;
        return nullptr;
    }

    mObjects[mObjectsCount] = res;
    mObjectsCount++;

    return res;
}

inline CSceneObject * const * CScene::GetObjects(void) const
{
    return mObjects;
}

inline unsigned int CScene::EnumObjects(void) const
{
    return mObjectsCount;
}

inline CSubsystem * const * CScene::GetSubsystems(void) const
{
    return mSubsystems;
}

inline unsigned int CScene::EnumSubsystems(void) const
{
    return mSubsystemsCount;
}

} // namespace drash

#endif // CSCENE_H
