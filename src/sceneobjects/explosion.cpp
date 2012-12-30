// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "explosion.h"

#include "../scene.h"
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
    mBoundingBox.lowerBound.Set(GetPos().Get().mX - mParams.mRadius, GetPos().Get().mY - mParams.mRadius);
    mBoundingBox.upperBound.Set(GetPos().Get().mX + mParams.mRadius, GetPos().Get().mZ + mParams.mRadius);
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
            CVec2f dist = GetScene()->GetObjects()[i]->GetPos().Get().Vec2();
            dist -= this->GetPos().Get().Vec2();

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
    DrawCircle(mParams.mRadius * (mParams.mLifeTime - mTime) / mParams.mLifeTime, CColor4f(1, 0, 0, 0.5));
}

}// namespace drash

