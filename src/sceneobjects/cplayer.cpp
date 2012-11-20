#include "cplayer.h"
#include "../diag/clogger.h"

#include "../cscene.h"

namespace drash
{

CPlayerEvent::CPlayerEvent( const PlayerAction &_action, const CVec2 &_mousePos ):
    mType(_action),
    mMousePos(_mousePos)
{
}

void CPlayerEvent::SetMousePos( const CVec2 &_pos )
{
    mMousePos = _pos;
}

CVec2 CPlayerEvent::GetMousePos() const
{
    return mMousePos;
}

const float CPlayer::mHeightJump = 10;

CPlayerParams::CPlayerParams():
    CSceneObjectParams(),
    mSpeedJump(10),
    mSpeedMoving(20),
    mPointShoot(3,10)
{

}

CPlayer::CPlayer():
    CSceneObject(),
    mJumpAllowed(false),
    mMovingLeft(false),
    mMovingRight(false)
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
    mSpeedMoving = _params.mSpeedMoving;
    mPointShoot = _params.mPointShoot;
    return true;
}

void CPlayer::Step( double _dt )
{
    CSceneObject::Step(_dt);

    if (mJumpAllowed)
    {
        if (mMovingLeft)
        {
            MoveLeft();
        }
        else if (mMovingRight)
        {
            MoveRight();
        }
    }
}

void CPlayer::Jump()
{
    if (mJumpAllowed)
	{
        CVec2 v = GetBody()->GetLinearVelocity();
        v.y = mSpeedJump;
        SetLinearVelocity(v);
    }
}

void CPlayer::MoveRight()
{
    CVec2 velocity = GetBody()->GetLinearVelocity();
    velocity.x = mSpeedMoving;
    SetLinearVelocity(velocity);
}

void CPlayer::MoveLeft()
{
    CVec2 velocity = GetBody()->GetLinearVelocity();
    velocity.x = -mSpeedMoving;
    SetLinearVelocity(velocity);
}

void CPlayer::FireNow( const CVec2 &_fireDirect )
{
}

void CPlayer::onEvent( const CPlayerEvent &_event )
{
    switch ( _event.mType )
	{
        case CPlayerEvent::PlayerActionJump:
			Jump();
			break;

        case CPlayerEvent::PlayerActionMoveLeft:
            mMovingLeft = true;
            break;

        case CPlayerEvent::PlayerActionMoveRight:
            mMovingRight = true;
            break;

        case CPlayerEvent::PlayerActionEndMoveLeft:
            mMovingLeft = false;
            break;

        case CPlayerEvent::PlayerActionEndMoveRight:
            mMovingRight = false;
            break;

        case CPlayerEvent::PlayerActionMoveDeep:
            for ( auto f = GetBody()->GetFixtureList(); f != NULL; f = f->GetNext() )
            {
                if (f->GetUserData() != nullptr)
                {
                    CFigure *fg = reinterpret_cast<CFigure*>(f->GetUserData());
                    fg->SetZet(fg->GetZet()-1);
                }
            }
            SetActive(false);
            SetActive(true);
            break;


        case CPlayerEvent::PlayerActionMoveOut:
            for ( auto f = GetBody()->GetFixtureList(); f != NULL; f = f->GetNext() )
            {
                if (f->GetUserData() != nullptr)
                {
                    CFigure *fg = reinterpret_cast<CFigure*>(f->GetUserData());
                    fg->SetZet(fg->GetZet()+1);
                }
            }            
            SetActive(false);
            SetActive(true);
            break;

        case CPlayerEvent::PlayerActionFire:
            FireNow( _event.GetMousePos() );
            break;
    }
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

}// namespace drash
