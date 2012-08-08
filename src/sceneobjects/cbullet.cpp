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
    this->SetDead(true);
    CBoomParams boom;
//    if (_contact.mPointCount == 2){
//        boom.mPos.Set((_contact.mPoints[0].x + _contact.mPoints[1].x) /2,
//                      (_contact.mPoints[0].y + _contact.mPoints[1].y) /2);
//    }else{
//        boom.mPos = _contact.mPoints[0];
//    }

    boom.mPos = GetPos();
    boom.mStregth = 5000;
    //LOG_INFO(boom.mPos.x << " " << boom.mPos.y);
    GetScene()->AddRequestBoom(boom);
}

CBulletParams::CBulletParams():
    CSolidBodyParams(),
    mTarget(0,0)
{
}

}// namespace drash
