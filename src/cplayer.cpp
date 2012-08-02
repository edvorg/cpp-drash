#include "cplayer.h"

namespace drash{

CPlayerParams::CPlayerParams():
    CSolidBodyParams()
{
}


CPlayer::CPlayer():
    CSolidBody()
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
}

}// namespace drash
