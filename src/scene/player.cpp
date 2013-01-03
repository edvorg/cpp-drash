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

#include "player.h"
#include "../diag/logger.h"
#include "scene.h"

namespace drash
{

CPlayerEvent::CPlayerEvent(const PlayerAction &_action):
    mType(_action)
{
}

bool CPlayer::Init(const GeometryT &_geometry, const CPlayer::ParamsT &_params )
{
    if ( CSceneObject::Init(_geometry, _params) == false )
	{
        return false;
    }

    SetFixedRotation(true);

    mSpeedJump = _params.mSpeedJump;
    return true;
}

void CPlayer::OnContactBegin(const CFigure *, const CFigure *)
{
}

void CPlayer::OnContactPreSolve(const CFigure *_f1, const CFigure *_f2)
{
    CSceneObject::OnContactBegin(_f1, _f2);
    mJumpAllowed = true;
}

void CPlayer::OnContactEnd(const CFigure *_f1, const CFigure *_f2)
{
    CSceneObject::OnContactEnd(_f1, _f2);
    mJumpAllowed = false;
}

void CPlayer::OnBoom(const CExplosionParams &)
{
}

void CPlayer::onEvent( const CPlayerEvent &_event )
{
    CVec3f new_target;
    auto move = [&new_target, this] (const CVec3f &_dir)
    {
        if (GetPos().IsTargetSet())
        {
            new_target = GetPos().GetTarget();
        }
        else
        {
            new_target = GetPos().Get();
		}
        new_target += _dir;
        GetPos().SetTarget(new_target, 0.5, AnimationBehaviorSingle);
    };

    switch ( _event.mType )
    {
        case CPlayerEvent::PlayerActionJump:
            if (mJumpAllowed)
            {
                move(CVec3f(0, mSpeedJump, 0));
            }
            break;

        case CPlayerEvent::PlayerActionMoveLeft:
            move(CVec3f(-1, 0, 0));
            break;

        case CPlayerEvent::PlayerActionMoveRight:
            move(CVec3f(1, 0, 0));
            break;

        case CPlayerEvent::PlayerActionMoveDeep:
            move(CVec3f(0, 0, -5));
            break;

        case CPlayerEvent::PlayerActionMoveOut:
            move(CVec3f(0, 0, 5));
            break;
    }
}

}// namespace drash
