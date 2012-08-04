#include "cbullet.h"

namespace drash{

CBullet::CBullet():
    CSolidBody()
{

}

bool CBullet::Init(const ParamsT &_params)
{
    if (CSolidBody::Init(_params) == false){
        return false;
    }

    GetBody()->SetBullet(false);

    CVec2 v = _params.mTarget;
    v.x -= _params.mPos.x;
    v.y -= _params.mPos.y;
    v *= 30.0;
    GetBody()->SetLinearVelocity(v);
    return true;
}


void CBullet::BeginContact(const drash::CContact &_contact)
{

}

CBulletParams::CBulletParams():
    CSolidBodyParams(),
    mTarget(0,0)
{
}

}// namespace drash
