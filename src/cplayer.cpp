#include "cplayer.h"

namespace drash{

CPlayerParams::CPlayerParams():
    CSceneObjectParams()
{
}


CPlayer::CPlayer():
    CSceneObject()
{

}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init(const CPlayer::ParamsT &_params)
{
    if ( CSceneObject::Init(_params) == false ){
        return false;
    }

    return true;
}

void CPlayer::Release()
{
    CSceneObject::Release();
}

void CPlayer::Step(double _dt)
{
    CSceneObject::Step(_dt);
}

}// namespace drash
