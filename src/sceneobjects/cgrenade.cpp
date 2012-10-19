#include "cgrenade.h"

#include "../cscene.h"

namespace drash
{

CGrenadeParams::CGrenadeParams():
    CBulletParams(),
    mTime(1.0)
{}

CGrenade::CGrenade():
    CBullet(),
    mTime(1.0),
    mTimer(),
    mCounter(0)
{
}

bool CGrenade::Init( const CGrenade::ParamsT &_params )
{
    if ( CBullet::Init(_params) == false )
    {
        return false;
    }

    mTime = _params.mTime;

    return true;
}

void CGrenade::OnContactBegin( const CContact &_contact )
{
    if ( mCounter == 0 )
    {
        mCounter++;
        mTimer.Reset(true);
    }
}

void CGrenade::Step( double _dt )
{
    CBullet::Step(_dt);

    if ( mCounter == 1 )
    {
        mTimer.Tick();

        if ( mTimer.GetFullTime() >= mTime )
        {
            mCounter++;
            CBoomParams boom;
            boom.mPos = mPos.Get();
            boom.mStregth = 500;
            GetScene()->AddRequestBoom(boom);
            SetDead();
        }
    }
}


}// namespace drash
