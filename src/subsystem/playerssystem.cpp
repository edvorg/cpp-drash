#include "playerssystem.h"

#include <diag/clogger.h>

namespace drash
{

CPlayersSystem::CPlayersSystem():
    mPlayersCount(0)
{
}

int CPlayersSystem::AddPlayer(CPlayerParams &_params)
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

    CPlayer *p = GetScene()->CreateObject<CPlayer>(_params);

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

    if (_player < 0 && mPlayersCount <= _player)
    {
        LOG_ERR("CPlayersSystem::RemPlayer(): player does not exist");
        return;
    }

    GetScene()->DestroyObject(mPlayers[_player]);
}

const CPlayersSystem::CPlayerPtr *CPlayersSystem::GetPlayers()
{
    return mPlayers;
}

unsigned int CPlayersSystem::EnumPlayers() const
{
    return mPlayersCount;
}

}// namespace drash
