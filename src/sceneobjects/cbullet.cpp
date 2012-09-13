#include "cbullet.h"

#include "../cscene.h"

namespace drash
{

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

    b2Filter filter;
    filter.categoryBits = 0x0010;
    GetBody()->GetFixtureList()->SetFilterData(filter);

    CVec2 v = _params.mTarget;
    v.x -= _params.mPos.x;
    v.y -= _params.mPos.y;
    v *= 30.0;
    GetBody()->SetLinearVelocity(v);
    return true;
}


void CBullet::BeginContact(const CContact &_contact)
{
}

void CBullet::PostSolve(const CContact &_contact)
{
    this->SetDead();
    CBoomParams boom;
    boom.mPos = mPos.Get();
    boom.mStregth = 50000;
    GetScene()->AddRequestBoom(boom);
}

void CBullet::Boom(const CBoomParams &_boom)
{
}

CBulletParams::CBulletParams():
    CSolidBodyParams(),
    mTarget(0,0)
{
}

}// namespace drash
