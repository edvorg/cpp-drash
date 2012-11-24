#include "cplayer.h"
#include "../diag/clogger.h"

#include "../cscene.h"

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

void CPlayer::OnContactBegin( const CContact &_contact )
{
}

void CPlayer::OnContactPreSolve(const CContact &_contact)
{
    CSceneObject::OnContactBegin(_contact);
    mJumpAllowed = true;
}

void CPlayer::OnContactEnd( const CContact &_contact )
{
    CSceneObject::OnContactEnd(_contact);
    mJumpAllowed = false;
}

void CPlayer::OnBoom( const CExplosionParams &_boom )
{
}

void CPlayer::onEvent( const CPlayerEvent &_event )
{
    switch ( _event.mType )
    {
        case CPlayerEvent::PlayerActionJump:
            if (mJumpAllowed)
            {
                CVec2 v = GetBody()->GetLinearVelocity();
                v.y = mSpeedJump;
                SetLinearVelocity(v);
            }
            break;

        case CPlayerEvent::PlayerActionMoveLeft:
            ApplyLinearImpulse(CVec2(-10, 0), CVec2(0));
            break;

        case CPlayerEvent::PlayerActionMoveRight:
            ApplyLinearImpulse(CVec2(10, 0), CVec2(0));
            break;

        case CPlayerEvent::PlayerActionMoveDeep:
            SetZTarget(GetZ().Get()-5, 0.5, AnimationBehaviorSingle);
            SetActive(false);
            SetActive(true);
            break;

        case CPlayerEvent::PlayerActionMoveOut:
            SetZTarget(GetZ().Get()+5, 0.5, AnimationBehaviorSingle);
            SetActive(false);
            SetActive(true);
            break;
    }
}

}// namespace drash
