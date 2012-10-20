#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "sceneobjects.h"
#include <list>
#include "diag/assert.h"

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
};

class CScene : public b2ContactListener, public b2ContactFilter
{
public:
    CScene(void);
    virtual ~CScene(void);

    bool Init( const CSceneParams& _params );
    void Release(void);

    /// T must extend CSceneObject class
    /// Method excepts, that T has ParamsT typedef
    template < typename T >
    T* CreateObject( const typename T::ParamsT& _params );

    /// T must extend CSceneObject class
    /// Method excepts, that T has ParamsT typedef
    template < typename T >
    void DestroyObject( T* _obj );

    /// must be called once in update cycle
    /// dt - nanoseconds
    void Step( double _dt );    

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);

    virtual void BeginContact( b2Contact * _contact );
    virtual void PreSolve( b2Contact* _contact, const b2Manifold* _oldManifold );
    virtual void EndContact( b2Contact * _contact );

    void Draw( const CCamera &_camera );

    void OnPlayerEvent( const CPlayerEvent & _event, unsigned int _playerId );

    // returns player id ( >= 0 ) or -1 if failed
    int AddPlayer( const CPlayerParams & _params );
    CPlayer *GetPlayer( unsigned int _id );

    void AddRequestBoom( const CBoomParams  _boom );

    void BoomNow();
protected:

private:
    b2World mWorld;
    std::list<CBoomParams> mListBooms;
    bool mInitialized;
    unsigned int mCountPlayers;
    unsigned int mCountObjects;

    static const int mVelocityIterations = 5;
    static const int mPositionIterations = 2;
    static const unsigned int mPlayersMaxAmount = 4;
    static const unsigned int mObjectsMaxAmount = 5000;

    CPlayer* mPlayers[mPlayersMaxAmount];
    CSceneObject* mObjects[mObjectsMaxAmount];
};

template < typename T >
T* CScene::CreateObject( const typename T::ParamsT& _params )
{
    if (mCountObjects == mObjectsMaxAmount){
        LOG_ERR("CScene::CreateObject() : Achieved maximum Amount of Objects in scene");
        return NULL;
    }

    T* res = new T();

    b2BodyDef bdef;
    res->mBody = mWorld.CreateBody(&bdef);    

    if ( res->mBody == NULL || res->Init(_params) == false )
    {
        delete res;
        return NULL;
    }

    res->mBody->SetUserData(res);
    res->mScene = this;
    res->mInternalId = mCountObjects;

    mObjects[mCountObjects] = res;
    mCountObjects++;
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

    mCountObjects--;
    mObjects[_obj->mInternalId] = mObjects[mCountObjects];
    mObjects[mCountObjects] = NULL;

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
