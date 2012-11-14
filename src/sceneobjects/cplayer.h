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
        PlayerActionEndMoveLeft,
        PlayerActionEndMoveRight,
        PlayerActionJump,
        PlayerActionFire
    };

    CPlayerEvent( const PlayerAction & _action , const CVec2 &_mousePos );


    PlayerAction mType;
    void SetMousePos( const CVec2 &_pos );
    CVec2 GetMousePos() const;

private:
    CVec2 mMousePos;
};

class CPlayerParams: public CSceneObjectParams
{
public:
    float mSpeedJump;
    float mSpeedMoving;
    CVec2 mPointShoot;
    CPlayerParams();
};

class CPlayer: public CSceneObject
{
public:
    friend class CScene;
    friend class CPlayersSystem;

    typedef CPlayerParams ParamsT;

protected:
    CPlayer();

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

    virtual void Step( double _dt );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnContactPreSolve( const CContact &_contact );
    virtual void OnContactEnd( const CContact &_contact );
    virtual void OnBoom( const CExplosionParams &_boom );

    virtual void onEvent( const CPlayerEvent & _event );

    void Jump();
    void MoveRight();
    void MoveLeft();
    void FireNow( const CVec2 & _fireDirect );
    bool mJumpAllowed;
    bool mMovingLeft;
    bool mMovingRight;
    float mSpeedJump;
    float mSpeedMoving;
    CVec2 mPointShoot;


    static const float mHeightJump;
};

}// namespace drash
#endif // CPLAYER_H
