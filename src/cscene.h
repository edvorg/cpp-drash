#ifndef CSCENE_H
#define CSCENE_H

#include "graphics/cdebugrenderer.h"
#include "ccontactlistener.h"
#include "sceneobjects.h"
#include <list>

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
};

class CScene
{
public:
    typedef std::list<CSceneObject*> ObjectsT;

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

    void SetDebugRenderer( CDebugRenderer *_renderer );
    void Draw(void);

    void OnPlayerEvent(const CPlayer::PlayerEvent & _event, unsigned int _playerId);

    void AddPlayer();
protected:

private:
    b2World mWorld;
    bool mInitialized;
    unsigned int mCountPlayers;
    static const int mVelocityIterations = 5;
    static const int mPositionIterations = 2;
    static const unsigned int mPlayersMaxAmount = 4;
    CContactListener mContactListener;
    ObjectsT mObjects;
    CPlayer* mPlayers[mPlayersMaxAmount];
};

template < typename T >
T* CScene::CreateObject( const typename T::ParamsT& _params )
{
    T* res = new T();

    b2BodyDef bdef;
    res->mBody = mWorld.CreateBody(&bdef);
    if ( res->mBody == NULL || res->Init(_params) == false )
    {
        delete res;
        return NULL;
    }

    mObjects.push_back(res);

    return res;
}

template < typename T >
void CScene::DestroyObject( T* _obj )
{
    for ( ObjectsT::iterator i=mObjects.begin(), i_e=mObjects.end(); i!=i_e; i++ )
    {
        if ( (*i) == _obj )
        {
            // ok, it's our object

            b2Body* body = _obj->mBody;
            body->SetUserData(NULL);

            mObjects.erase(i);
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
