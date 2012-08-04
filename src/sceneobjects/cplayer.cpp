#include "cplayer.h"
#include "../clogger.h"
namespace drash{

const float CPlayer::mHeightJump = 10;

CPlayerParams::CPlayerParams():
    CSolidBodyParams()
{
    mFixedRotation = true;
    mFriction = 1;
}

CPlayer::CPlayer():
    CSolidBody(),
    mJumping(false)
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

    return true;
}

void CPlayer::Release()
{
    CSolidBody::Release();
}

void CPlayer::Step(double _dt)
{
    CSolidBody::Step(_dt);

    mJumping = mTargetSet;
}

void CPlayer::Jump()
{
    MoveRight();
    CVec2 v = mBody->GetLinearVelocity();
    v.y += 15;
    mBody->SetLinearVelocity(v);
}

void CPlayer::MoveRight()
{
    CVec2 force(15,0);
    ///mBody->ApplyForce(force,CVec2(0,0));

    mBody->SetLinearVelocity(force);
}

void CPlayer::MoveLeft()
{
}
// TODO: release move player
// TODO: release fire player
void CPlayer::onEvent(const PlayerEvent &_event){
    switch (_event){
        case jump:Jump() ;break;
        case moveLeft: break;
        case moveRight: break;
        case fire: break;
    }
}

void CPlayer::BeginContact(CSceneObject *_object)
{
    mJumping = true;
}

void CPlayer::EndContact(CSceneObject *_object)
{
    mJumping = false;
}

}// namespace drash
