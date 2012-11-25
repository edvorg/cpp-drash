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

#ifndef PLAYERSSYSTEM_H
#define PLAYERSSYSTEM_H

#include <subsystem/subsystem.h>
#include <sceneobjects/cplayer.h>

namespace drash
{

class CPlayersSystem : public CSubsystem
{
public:
    typedef CPlayer *CPlayerPtr;

    int AddPlayer(const CSceneObjectGeometry &_geometry, const CPlayerParams &_params);
    void RemPlayer(int _player);
    inline const CPlayerPtr *GetPlayers();
    inline unsigned int EnumPlayers() const;

    void OnPlayerEvent( const CPlayerEvent & _event, unsigned int _playerId );

    static const unsigned int mMaxPlayersCount = 10;

private:
    CPlayerPtr mPlayers[mMaxPlayersCount];
    unsigned mPlayersCount = 0;
};

inline const CPlayersSystem::CPlayerPtr *CPlayersSystem::GetPlayers()
{
    return mPlayers;
}

inline unsigned int CPlayersSystem::EnumPlayers() const
{
    return mPlayersCount;
}

}// namespace drash

#endif // PLAYERSSYSTEM_H
