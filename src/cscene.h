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
    /// T must extend CSceneObject class

    template < typename T >
    void DestroyObject( T* _obj );

    const ObjectsT &GetObjects(void) const;

    /// returns total objects count
    unsigned int EnumObjects(void) const;

    /// destroyes all objects
    void DestroyObjects(void);

    // **************************************************
    // * working with subsystems ************************

    /// as we made a connection, _subsystem gets a pointer to our CScene instance
    /// and starts interaction with it
    void ConnectSubsystem(CSubsystem *_subsystem);

    /// as we made disconnect _subsystem, we lose pointer to this CScene instance
    void DisconnectSubsystem(CSubsystem *_subsystem);

    const SystemsT &GetSubsystems(void) const;

    /// returns total connected subsystems count
    unsigned int EnumSubsystems(void) const;

protected:
private:
    template < typename T >
    void DestroyObjectImpl( T* _obj );

    bool mInitialized;
    bool mLocked;

    b2World mWorld;
    CPhysObserver mObserver;

    ObjectsT mObjects;
    unsigned int mObjectsCount;

    SystemsT mSubsystems;
    unsigned int mSubsystemsCount;
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

template < typename T >
void CScene::DestroyObject( T* _obj )
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

template < typename T >
void CScene::DestroyObjectImpl( T* _obj )
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

#endif // CSCENE_H
