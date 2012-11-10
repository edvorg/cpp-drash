#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "sceneobjects.h"
#include "diag/assert.h"

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
};

class CSubsystem;

class CScene : public b2ContactListener, public b2ContactFilter
{
public:
    typedef CSceneObject *CSceneObjectPtr;
    typedef const CSceneObject *CSceneObjectConstPtr;
    typedef CSubsystem *CSubsystemPtr;
    typedef const CSubsystem *CSubsystemConstPtr;

    CScene(void);
    virtual ~CScene(void);

    bool Init( const CSceneParams& _params );
    void Release(void);

    /// T must extend CSceneObject class
    /// Method excepts, that T has ParamsT typedef
    template < typename T >
    T* CreateObject( const typename T::GeometryT &_geometry, const typename T::ParamsT& _params );

    /// T must extend CSceneObject class
    /// Method excepts, that T has ParamsT typedef
    template < typename T >
    void DestroyObject( T* _obj );

    const CSceneObjectPtr* GetObjects();
    unsigned int EnumObjects() const;

    /// must be called once in update cycle
    /// dt - nanoseconds
    void Step( double _dt );    

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);

    virtual void BeginContact( b2Contact * _contact );
    virtual void PreSolve( b2Contact* _contact, const b2Manifold* _oldManifold );
    virtual void EndContact( b2Contact * _contact );

    void Draw( const CCamera &_camera );

    void ConnectSubsystem(CSubsystem *_subsystem);
    void DisconnectSubsystem(CSubsystem *_subsystem);
    const CSubsystemPtr *GetSubsystems();
    unsigned int EnumSubsystems() const;


    void Clear();
protected:

private:
    b2World mWorld;
    bool mInitialized;
    unsigned int mCountPlayers;
    unsigned int mObjectsCount;

    static const int mVelocityIterations = 5;
    static const int mPositionIterations = 2;
    static const unsigned int mObjectsMaxAmount = 5000;
    static const unsigned int mMaxSubsystemsCount = 3;

    CSceneObject* mObjects[mObjectsMaxAmount];

    CSubsystem *mSubsystems[mMaxSubsystemsCount];
    unsigned int mSubsystemsCount;
};

template < typename T >
T* CScene::CreateObject(const typename T::GeometryT &_geometry, const typename T::ParamsT& _params)
{
    if (mObjectsCount == mObjectsMaxAmount){
        LOG_ERR("CScene::CreateObject() : Achieved maximum Amount of Objects in scene");
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
    DRASH_ASSERT( mObjects[_obj->mInternalId] == _obj &&
                  "something wrong with objects creation logic" );

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
