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
