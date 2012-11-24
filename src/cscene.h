#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "sceneobjects.h"
#include "physobserver.h"
#include "diag/assert.h"

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
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
    // * supporting types *******************************

    typedef CSceneObject *CSceneObjectPtr;
    typedef CSceneObjectPtr ObjectsT[mObjectsCountLimit];
    typedef CSubsystem *CSubsystemPtr;
    typedef CSubsystemPtr SystemsT[mSubsystemsCountLimit];

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

    inline const ObjectsT &GetObjects(void) const;

    /// returns total objects count
    inline unsigned int EnumObjects(void) const;

    /// destroyes all objects
    void DestroyObjects(void);

    // **************************************************
    // * working with subsystems ************************

    /// as we made a connection, _subsystem gets a pointer to our CScene instance
    /// and starts interaction with it
    void ConnectSubsystem(CSubsystem *_subsystem);

    /// as we made disconnect _subsystem, we lose pointer to this CScene instance
    void DisconnectSubsystem(CSubsystem *_subsystem);

    inline const SystemsT &GetSubsystems(void) const;

    /// returns total connected subsystems count
    inline unsigned int EnumSubsystems(void) const;

protected:
private:
    void DestroyObjectImpl(CSceneObject *_obj);

    bool mInitialized = false;
    bool mLocked = false;

    b2World mWorld;
    CPhysObserver mObserver;

    ObjectsT mObjects;
    unsigned int mObjectsCount = 0;

    SystemsT mSubsystems;
    unsigned int mSubsystemsCount = 0;
};

template < typename T >
T* CScene::CreateObject(const typename T::GeometryT &_geometry, const typename T::ParamsT& _params)
{
    if (mObjectsCount == mObjectsCountLimit)
	{
        LOG_ERR("CScene::CreateObject(): Achieved maximum Amount of Objects in scene");
        return NULL;
    }

    if (mWorld.IsLocked())
    {
        LOG_ERR("CScene::CreateObject(): world is locked now");
        return NULL;
    }

    T* res = new T();

    b2BodyDef bdef;
    res->mBody = mWorld.CreateBody(&bdef);    

    if ( res->mBody == NULL || res->Init(_geometry, _params) == false )
    {
        delete res;
        return NULL;
    }

    res->mBody->SetUserData(res);
    res->mScene = this;
    res->mInternalId = mObjectsCount;

    mObjects[mObjectsCount] = res;
    mObjectsCount++;
    return res;
}

inline const CScene::ObjectsT &CScene::GetObjects(void) const
{
    return mObjects;
}

inline unsigned int CScene::EnumObjects(void) const
{
    return mObjectsCount;
}

inline const CScene::SystemsT &CScene::GetSubsystems(void) const
{
    return mSubsystems;
}

inline unsigned int CScene::EnumSubsystems(void) const
{
    return mSubsystemsCount;
}

} // namespace drash

#endif // CSCENE_H
