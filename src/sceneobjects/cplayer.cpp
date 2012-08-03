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

void CPlayer::Jump()
{
    if (mJumping == false){

    }
}
// TODO: release move player
// TODO: release fire player
void CPlayer::onEvent(const PlayerEvent &_event){
    switch (_event){
        case jump: ;break;
        case moveLeft: break;
        case moveRight: break;
        case fire: break;
    }
}

}// namespace drash
