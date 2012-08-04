#include "cplayer.h"
#include "../clogger.h"
namespace drash{

const float CPlayer::mHeightJump = 10;

CPlayerParams::CPlayerParams():
    CSolidBodyParams(),
    mSpeedJump(10),
    mSpeedMoving(20)
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

bool CPlayer::Init(const CPlayer::ParamsT &_params)
{
    if ( CSolidBody::Init(_params) == false ){
        return false;
    }

    GetBody()->SetFixedRotation(true);
    GetBody()->GetFixtureList()->SetFriction(10);
    mSpeedJump = _params.mSpeedJump;
    mSpeedMoving = _params.mSpeedMoving;

    return true;
}

void CPlayer::Release()
{
    CSolidBody::Release();
}

void CPlayer::Step(double _dt)
{
    CSolidBody::Step(_dt);
    if (mMovingLeft){
        MoveLeft();
    }
    if (mMovingRight){
        MoveRight();
    }
}

void CPlayer::Jump()
{
    if (mJumpAllowed){
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

void CPlayer::FireNow()
{

}

// TODO: release fire player
void CPlayer::onEvent(const CPlayerEvent &_event){
    switch (_event.mType){
        case CPlayerEvent::jump:Jump() ;break;
        case CPlayerEvent::StartMoveLeft: mMovingLeft = true; break;
        case CPlayerEvent::StartMoveRight: mMovingRight = true; break;
        case CPlayerEvent::EndMoveLeft: mMovingLeft = false; break;
        case CPlayerEvent::EndMoveRight: mMovingRight = false; break;
        case CPlayerEvent::fire: ;break;
    }
}

void CPlayer::BeginContact(CSceneObject *_object, const b2Manifold *_manifold)
{
    CSolidBody::BeginContact(_object, _manifold);
    mJumpAllowed = true;
}

void CPlayer::EndContact(CSceneObject *_object, const b2Manifold *_manifold)
{
    CSolidBody::BeginContact(_object, _manifold);
    mJumpAllowed = false;
}

}// namespace drash
