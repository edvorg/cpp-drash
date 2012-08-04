#include "cplayer.h"
#include "../clogger.h"
#include "cbullet.h"
namespace drash{

const float CPlayer::mHeightJump = 10;

CPlayerParams::CPlayerParams():
    CSolidBodyParams(),
    mSpeedJump(10),
    mSpeedMoving(20),
    mPointShoot(10,0)
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

void CPlayer::FireNow(const CVec2 &_fireDirect)
{
    CBulletParams bulletParams;

    bulletParams.mTarget = _fireDirect;
    CVec2 posBody = GetBody()->GetWorldPoint(CVec2(0,0));
    if (_fireDirect.x < posBody.x){
        LOG_INFO("Hello");
        bulletParams.mPos.Set(posBody.x - 30,posBody.y);
    } else {
        bulletParams.mPos.Set(posBody.x + 10, posBody.y);
    }

    bulletParams.mPos = GetBody()->GetWorldPoint(CVec2(1,3));

    GetScene()->CreateObject< CBullet >( bulletParams );
}

// TODO: release fire player
void CPlayer::onEvent(const CPlayerEvent &_event){
    switch (_event.mType){
        case CPlayerEvent::jump:Jump() ;break;
        case CPlayerEvent::StartMoveLeft: mMovingLeft = true; break;
        case CPlayerEvent::StartMoveRight: mMovingRight = true; break;
        case CPlayerEvent::EndMoveLeft: mMovingLeft = false; break;
        case CPlayerEvent::EndMoveRight: mMovingRight = false; break;
        case CPlayerEvent::fire: FireNow(_event.GetPosMouse());break;
    }
}

void CPlayer::BeginContact(const CContact &_contact)
{
    CSolidBody::BeginContact(_contact);
    mJumpAllowed = true;
}

void CPlayer::EndContact(const CContact &_contact)
{
    CSolidBody::BeginContact(_contact);
    mJumpAllowed = false;
}

}// namespace drash
