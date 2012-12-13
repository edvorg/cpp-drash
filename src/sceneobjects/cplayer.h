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

    virtual void OnContactBegin(const CFigure *, const CFigure *) override;
    virtual void OnContactPreSolve(const CFigure *_f1, const CFigure *_f2) override;
    virtual void OnContactEnd(const CFigure *_f1, const CFigure *_f2) override;
    virtual void OnBoom(const CExplosionParams &) override;

    void onEvent( const CPlayerEvent & _event );

private:
    bool mJumpAllowed = false;
    float mSpeedJump = 0;
    constexpr static const float mHeightJump = 10;
};

}// namespace drash
#endif // CPLAYER_H
