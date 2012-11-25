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
