#include "cplayer.h"
#include "../diag/clogger.h"
#include "cbullet.h"
#include "cgrenade.h"

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
    CSolidBodyParams(),
    mSpeedJump(10),
    mSpeedMoving(20),
    mPointShoot(3,10)
{

}

CPlayer::CPlayer():
    CSolidBody(),
    mJumpAllowed(false),
    mMovingLeft(false),
    mMovingRight(false)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init( const CPlayer::ParamsT &_params )
{
    if ( CSolidBody::Init(_params) == false )
	{
        return false;
    }

    GetBody()->SetFixedRotation(true);
    GetBody()->GetFixtureList()->SetFriction(10);

    b2Filter filter;
    filter.maskBits = 0xffff ^ 0x0010;
    GetBody()->GetFixtureList()->SetFilterData(filter);

    mSpeedJump = _params.mSpeedJump;
    mSpeedMoving = _params.mSpeedMoving;
    mPointShoot = _params.mPointShoot;
    return true;
}

void CPlayer::Release()
{
    CSolidBody::Release();
}

void CPlayer::Step( double _dt )
{
    CSolidBody::Step(_dt);

    if (mMovingLeft)
	{
        MoveLeft();
    }
	else if (mMovingRight)
	{
        MoveRight();
    }
}

void CPlayer::Jump()
{
    if (mJumpAllowed)
	{
        CVec2 v = GetBody()->GetLinearVelocity();
        v.y = mSpeedJump;
        GetBody()->SetLinearVelocity(v);
    }
}

void CPlayer::MoveRight()
{
    CVec2 velocity = GetBody()->GetLinearVelocity();
    velocity.x = mSpeedMoving;
    GetBody()->SetLinearVelocity(velocity);
}

void CPlayer::MoveLeft()
{
    CVec2 velocity = GetBody()->GetLinearVelocity();
    velocity.x = -mSpeedMoving;
    GetBody()->SetLinearVelocity(velocity);
}

void CPlayer::FireNow( const CVec2 &_fireDirect )
{
    CGrenadeParams bulletParams;

    bulletParams.mTarget = _fireDirect;
    CVec2 posBody = GetBody()->GetWorldPoint( CVec2( 0, 0 ) );

    if ( _fireDirect.x > posBody.x )
	{
        bulletParams.mPos = GetBody()->GetWorldPoint(mPointShoot);
    }
	else
	{
        bulletParams.mPos = GetBody()->GetWorldPoint( CVec2( -mPointShoot.x,
															 mPointShoot.y ) );
    }

    bulletParams.mTime = 2;
    GetScene()->CreateObject<CGrenade>(bulletParams);
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

        case CPlayerEvent::PlayerActionFire:
            FireNow( _event.GetMousePos() );
            break;
    }
}

void CPlayer::OnContactBegin( const CContact &_contact )
{
    CSolidBody::OnContactBegin(_contact);
    mJumpAllowed = true;
}

void CPlayer::OnContactEnd( const CContact &_contact )
{
    CSolidBody::OnContactEnd(_contact);
    mJumpAllowed = false;
}

void CPlayer::OnBoom( const CBoomParams &_boom )
{
}

}// namespace drash
