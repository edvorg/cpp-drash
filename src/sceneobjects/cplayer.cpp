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

    mBody->SetFixedRotation(true);
    mBody->GetFixtureList()->SetFriction(10);
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
        CVec2 v = mBody->GetLinearVelocity();
        v.y = mSpeedJump;
        GetBody()->SetLinearVelocity(v);
    }
}

void CPlayer::MoveRight()
{
    CVec2 velocity = mBody->GetLinearVelocity();
    if (velocity.y > 0){
        velocity.x = mSpeedMoving;
    } else {
        velocity.x = mSpeedMoving;
    }
    GetBody()->SetLinearVelocity(velocity);
}

void CPlayer::MoveLeft()
{
    CVec2 velocity = mBody->GetLinearVelocity();
    if (velocity.y > 0){
        velocity.x = -mSpeedMoving;
    } else {
        velocity.x = -mSpeedMoving;
    }
    GetBody()->SetLinearVelocity(velocity);
}

// TODO: release fire player
void CPlayer::onEvent(const PlayerEvent &_event){
    switch (_event){
        case jump:Jump() ;break;
        case StartMoveLeft: mMovingLeft = true; break;
        case StartMoveRight: mMovingRight = true; break;
        case EndMoveLeft: mMovingLeft = false; break;
        case EndMoveRight: mMovingRight = false; break;
        case fire: ;break;
    }
}

void CPlayer::BeginContact(CSceneObject *_object)
{
    CSolidBody::BeginContact(_object);
    mJumpAllowed = true;
}

void CPlayer::EndContact(CSceneObject *_object)
{
    CSolidBody::BeginContact(_object);
    mJumpAllowed = false;
}

}// namespace drash
