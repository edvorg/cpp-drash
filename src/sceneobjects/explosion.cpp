#include "explosion.h"

#include <cscene.h>

namespace drash
{

CExplosionParams::CExplosionParams():
    mStregth(1),
    mLifeTime(1)
{
}

CExplosion::CExplosion():
    CSceneObject(),
    mParams(),
    mTime(0)
{
}

bool CExplosion::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    mParams = _params;

    SetDynamic(false);

    return true;
}

void CExplosion::Step(double _dt)
{
    mTime += _dt;

    if (mTime > mParams.mLifeTime)
    {
        SetDead();
    }

    for (unsigned int i=0; i<GetScene()->EnumObjects(); i++)
    {
        GetScene()->GetObjects()[i]->OnBoom(mParams);
    }
}

const CExplosionParams &CExplosion::GetParams() const
{
    return mParams;
}

float CExplosion::GetTime() const
{
    return mTime;
}

}// namespace drash

