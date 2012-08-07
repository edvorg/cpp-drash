#include "cscene.h"
#include "clogger.h"

namespace drash
{

CScene::CScene(void):
    mWorld( b2Vec2( 0, 0 ) ),
    mInitialized(false),
    mCountPlayers(0),
    mCountObjects(0)
{
}

CScene::~CScene(void)
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::~CScene(): warning Release() called from destructor" );
        Release();
    }
}

bool CScene::Init(const CSceneParams &_params)
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::Init(): already initialized" );
        return false;
    }

    mWorld.SetContactListener(&mContactListener);
    mWorld.SetAllowSleeping(true);
    mWorld.SetContinuousPhysics(false);
    mWorld.SetGravity(_params.mGravity);

    mInitialized = true;
    return true;
}

void CScene::Release(void)
{
    if ( mInitialized == false )
    {
        LOG_WARN( "CScene::Release(): already released" );
        return;
    }

    if (mCountObjects != 0)
    {
        LOG_WARN( "CScene::Release(): "<<
                  mCountObjects<<
                  " object(s) haven't been destroyed. Autorelease" );

        while(mCountObjects != 0)
        {
            DestroyObject< CSceneObject >( mObjects[0] );
        }
    }

    mInitialized = false;
}

void CScene::Step( double _dt )
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): this is not initialized" );
        return;
    }

    mWorld.Step( _dt, mVelocityIterations, mPositionIterations );

    for (unsigned int i = 0 ; i < mCountObjects ; i++){
        mObjects[i]->Step(_dt);
    }
    // TODO: delete objects, which dead


}

void CScene::SetDebugRenderer( CDebugRenderer *_renderer )
{
    mWorld.SetDebugDraw(_renderer);
}

void CScene::Draw(void)
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): this is not initialized" );
        return;
    }

    mWorld.DrawDebugData();
}

void CScene::OnPlayerEvent(const CPlayerEvent &_event, unsigned int _playerId)
{
    if (_playerId >= mCountPlayers){
        LOG_ERR("CScene::OnPlayerEvent() : Player with id = " <<
                _playerId << " no exist");

        return;
    }
    mPlayers[_playerId]->onEvent(_event);
}

void CScene::AddPlayer(const CPlayerParams &_params)
{
    if (mCountPlayers == mPlayersMaxAmount){
        LOG_ERR("CScene::AddPlayer() : Achieved maximum Amount of Players");
        return;
    }

    mPlayers[mCountPlayers++] = CreateObject<CPlayer>(_params);

}

} // namespace drash
