#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "graphics/cdebugrenderer.h"
#include "sceneobjects.h"
#include "cboomparams.h"
#include <list>

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
};

class CScene : public b2ContactListener
{
public:
    CScene(void);
    virtual ~CScene(void);

    bool Init( const CSceneParams& _params );
    void Release(void);

    /// T must extend CSceneObject class
    template < typename T >
    T* CreateObject( const typename T::ParamsT& _params );

    /// T must extend CSceneObject class
    template < typename T >
    void DestroyObject( T* _obj );

    /// must be called once in update cycle
    /// dt - nanoseconds
    void Step( double _dt );    

    virtual void BeginContact( b2Contact * _contact );
    virtual void PreSolve( b2Contact* _contact, const b2Manifold * _oldManifold );
    virtual void PostSolve( b2Contact * _contact, const b2ContactImpulse * _impulse);
    virtual void EndContact( b2Contact * _contact );

    void SetDebugRenderer( CDebugRenderer *_renderer );
    void Draw(void);

    void OnPlayerEvent(const CPlayerEvent & _event, unsigned int _playerId);

    // returns player id ( >= 0 ) or -1 if failed
    int AddPlayer( const CPlayerParams & _params );

    void AddRequestBoom(const CBoomParams  _boom);

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

    mObjects[mCountObjects] = res;
    mCountObjects++;
    return res;
}

template < typename T >
void CScene::DestroyObject( T* _obj )
{
    for (unsigned int i = 0 ; i < mCountObjects ; i++)
    {
        if (mObjects[i] == _obj)
        {
            // ok, it's our object

            b2Body* body = _obj->mBody;
            body->SetUserData(NULL);

            mObjects[i] = (mCountObjects > 1) ? mObjects[mCountObjects-1] : NULL;
            mCountObjects--;
            _obj->Release();
            delete _obj;

            for ( b2Body *b = mWorld.GetBodyList(); b; b = b->GetNext() )
            {
                if ( b == body )
                {
                    // ok, it's out body

                    while ( b2Fixture* f = b->GetFixtureList() )
                    {
                        b->DestroyFixture(f);
                    }

                    mWorld.DestroyBody(body);
                    break;
                }
            }
            break;
        }
    }
}

} // namespace drash

#endif // CSCENE_H
