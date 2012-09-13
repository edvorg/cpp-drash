#include "cgrenade.h"

#include "../cscene.h"

namespace drash{


CGrenadeParams::CGrenadeParams():
    CBulletParams(),
    mBoomTime(3)
{}

CGrenade::CGrenade():
    CBullet(),
    mBoomTime(3),
    mActivatedTimer(false)
{
}

bool CGrenade::Init(const CGrenade::ParamsT &_params)
{
    if ( CBullet::Init(_params) == false){
        return false;
    }

    mBoomTime = _params.mBoomTime;

    return true;
}

void CGrenade::BeginContact(const CContact &_contact)
{

}

void CGrenade::PostSolve(const CContact &_contact)
{
    if ( mActivatedTimer == false ){
        mActivatedTimer = true;
        mTimerOfBoom.Reset(true);
    }
}

void CGrenade::Boom(const CBoomParams &_boom)
{
    CBullet::Boom(_boom);
}

void CGrenade::Step(double _dt)
{
    CBullet::Step(_dt);
    if (mActivatedTimer == true){
        mTimerOfBoom.Tick();
        if (mTimerOfBoom.GetFullTime() >= mBoomTime){
            CBoomParams boom;
            boom.mPos = mPos.Get();
            boom.mStregth = 500000;
            GetScene()->AddRequestBoom(boom);
        }
    }
}


}// namespace drash
