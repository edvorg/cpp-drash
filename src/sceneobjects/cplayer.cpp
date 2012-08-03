#include "cplayer.h"

namespace drash{
const float CPlayer::mHeightJump = 10;
CPlayerParams::CPlayerParams():
    CSolidBodyParams()
{
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

void CPlayer::mJump()
{
    if (mJumping == false){
        CVec2 temp(GetPos().x,GetPos().y + mHeightJump);
        this->SetPosTargetSpeed(0.4);
        SetPosTarget(temp);
        mJumping = true;
    }
}
// TODO: release move player
// TODO: release fire player
void CPlayer::onAction(const Action &_action){
    switch (_action){
        case jump: mJump();break;
        case moveLeft: break;
        case moveRight: break;
        case fire: break;
    }
}

}// namespace drash
