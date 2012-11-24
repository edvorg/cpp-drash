#include "playerssystem.h"

#include <diag/clogger.h>

namespace drash
{

int CPlayersSystem::AddPlayer(const CSceneObjectGeometry &_geometry, const CPlayerParams &_params)
{
    if (GetScene() == NULL)
    {
        return -1;
    }

    if (mPlayersCount >= mMaxPlayersCount)
    {
        LOG_ERR("CPlayersSystem::AddPlayer(): players count is maximal");
        return -1;
    }

    CPlayer *p = GetScene()->CreateObject<CPlayer>(_geometry, _params);

    if (p != NULL)
    {
        mPlayers[mPlayersCount] = p;
        return mPlayersCount++;
    }
    else
    {
        return -1;
    }
}

void CPlayersSystem::RemPlayer(int _player)
{
    if (GetScene() == NULL)
    {
        LOG_ERR("CPlayersSystem::RemPlayer(): scene is unconnected");
        return;
    }

    if (_player < 0 && (int)mPlayersCount <= _player)
    {
        LOG_ERR("CPlayersSystem::RemPlayer(): player does not exist");
        return;
    }

    GetScene()->DestroyObject(mPlayers[_player]);
}

void CPlayersSystem::OnPlayerEvent( const CPlayerEvent &_event, unsigned int _playerId )
{
    if ( _playerId >= mPlayersCount )
    {
        LOG_ERR( "CPlayersSystem::OnPlayerEvent(): "
                 "Player with id = " <<
                 _playerId <<
                 " no exist" );
        return;
    }

    mPlayers[_playerId]->onEvent(_event);
}

}// namespace drash
