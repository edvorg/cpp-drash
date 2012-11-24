#pragma once
#ifndef CPLAYER_H
#define CPLAYER_H

#include "csceneobject.h"

namespace drash
{

class CPlayerEvent
{
public:
    enum PlayerAction
    {
        PlayerActionMoveLeft,
        PlayerActionMoveRight,
        PlayerActionMoveDeep,
        PlayerActionMoveOut,
        PlayerActionJump,
    };

    CPlayerEvent(const PlayerAction & _action );

    PlayerAction mType;
private:
};

class CPlayerParams: public CSceneObjectParams
{
public:
    float mSpeedJump = 10;
    float mSpeedMoving = 20;
};

class CPlayer: public CSceneObject
{
public:
    friend class CScene;
    friend class CPlayersSystem;

    typedef CPlayerParams ParamsT;

protected:
    bool Init( const GeometryT &_geometry, const ParamsT &_params );

    virtual void OnContactBegin( const CContact &_contact ) override;
    virtual void OnContactPreSolve( const CContact &_contact ) override;
    virtual void OnContactEnd( const CContact &_contact ) override;
    virtual void OnBoom( const CExplosionParams &_boom ) override;

    void onEvent( const CPlayerEvent & _event );

private:
    bool mJumpAllowed = false;
    float mSpeedJump = 0;
    constexpr static const float mHeightJump = 10;
};

}// namespace drash
#endif // CPLAYER_H
