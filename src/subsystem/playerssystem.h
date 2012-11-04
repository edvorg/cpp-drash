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

    CPlayersSystem();

    int AddPlayer(CPlayerParams &_params);
    void RemPlayer(int _player);
    const CPlayerPtr *GetPlayers();
    unsigned int EnumPlayers() const;

    static const unsigned int mMaxPlayersCount = 10;

private:
    CPlayerPtr mPlayers[mMaxPlayersCount];
    unsigned mPlayersCount;
};

}// namespace drash

#endif // PLAYERSSYSTEM_H
