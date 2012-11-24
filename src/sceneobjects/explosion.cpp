#include "explosion.h"

#include "../cscene.h"
#include "../misc/graphics.h"

namespace drash
{

CExplosionParams::CExplosionParams():
    mStregth(1),
    mLifeTime(1),
    mRadius(-1)
{
}

void CExplosion::ComputeBoundingBox()
{
    mBoundingBox.lowerBound.Set(GetPos().Get().x - mParams.mRadius, GetPos().Get().y - mParams.mRadius);
    mBoundingBox.upperBound.Set(GetPos().Get().x + mParams.mRadius, GetPos().Get().y + mParams.mRadius);
}

CExplosion::CExplosion():
    CSceneObject(),
    mParams(),
    mTime(0)
{
}

bool CExplosion::Init(const GeometryT &_geometry, const ParamsT &_params )
{
    if ( !CSceneObject::Init(_geometry, _params) )
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
        GetScene()->DestroyObject(this);
    }

    for (unsigned int i=0; i<GetScene()->EnumObjects(); i++)
    {
        if (mParams.mRadius > 0.0f)
        {
            CVec2 dist = GetScene()->GetObjects()[i]->GetBody()->GetWorldCenter();
            dist -= this->GetBody()->GetWorldCenter();

            if (dist.LengthSquared() <= mParams.mRadius * mParams.mRadius)
            {
                GetScene()->GetObjects()[i]->OnBoom(mParams);
            }
        }
        else
        {
            GetScene()->GetObjects()[i]->OnBoom(mParams);
        }
    }
}

void CExplosion::DrawDebug() const
{
    DrawCircle(mParams.mRadius * (mParams.mLifeTime - mTime) / mParams.mLifeTime, 1, 0, 0, 0.5);
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

