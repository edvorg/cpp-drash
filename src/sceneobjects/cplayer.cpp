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

bool CPlayer::Init( const CPlayer::ParamsT &_params )
{
    if ( CSceneObject::Init(_params) == false )
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
//    CGrenadeParams bulletParams;

//    bulletParams.mTarget = _fireDirect;
//    CVec2 posBody = GetBody()->GetWorldPoint( CVec2( 0, 0 ) );

//    if ( _fireDirect.x > posBody.x )
//	{
//        bulletParams.mPos = GetBody()->GetWorldPoint(mPointShoot);
//    }
//	else
//	{
//        bulletParams.mPos = GetBody()->GetWorldPoint( CVec2( -mPointShoot.x,
//															 mPointShoot.y ) );
//    }

//    bulletParams.mTime = 2;
//    bulletParams.mFigures.resize(1);
//    float tmp = ( reinterpret_cast<CInterval*>( GetBody()->GetFixtureList()->GetUserData() )->GetMin() +
//                  reinterpret_cast<CInterval*>( GetBody()->GetFixtureList()->GetUserData() )->GetMax() ) / 2;
//    bulletParams.mFigures[0].mLayers.Set( tmp, tmp + 1 );
//    GetScene()->CreateObject<CGrenade>(bulletParams);
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
                auto i = reinterpret_cast<CInterval*>( f->GetUserData() );
                i->Set( i->GetMin() - 100, i->GetMax() - 100 );
            }
            SetActive(false);
            SetActive(true);
            break;


        case CPlayerEvent::PlayerActionMoveOut:
            for ( auto f = GetBody()->GetFixtureList(); f != NULL; f = f->GetNext() )
            {
                auto i = reinterpret_cast<CInterval*>( f->GetUserData() );
                i->Set( i->GetMin() + 100, i->GetMax() + 100 );
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

void CPlayer::OnBoom( const CBoomParams &_boom )
{
}

}// namespace drash
