// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

#include "plane.h"
#include "ray.h"

namespace drash
{

extern const CPlane PlaneXY(CVec3f(0), CVec3f(0, 0, 1));
extern const CPlane PlaneYZ(CVec3f(0), CVec3f(1, 0, 0));
extern const CPlane PlaneXZ(CVec3f(0), CVec3f(0, 1, 0));

CPlane::CPlane()
{
}

CPlane::CPlane(const CPlane &_plane):
    mPoint(_plane.mPoint),
    mNormal(_plane.mNormal),
    mD(_plane.mD)
{
}

CPlane::CPlane(const CVec3f &_point, const CVec3f &_normal)
{
    mPoint = _point;
    mNormal = _normal;
    mNormal.Normalize();
    ComputeD();
}

void CPlane::SetPoint(const CVec3f &_point)
{
    mPoint = _point;
    ComputeD();
}

void CPlane::SetNormal(const CVec3f &_normal)
{
    mNormal = _normal;
    mNormal.Normalize();
    ComputeD();
}

void CPlane::Set(const CVec3f &_p1, const CVec3f &_p2, const CVec3f &_p3)
{
    mPoint = _p1;
    CVec3f a(_p2);
    CVec3f b(_p3);
    a -= _p1;
    b -= _p1;
    Vec3Cross(a, b, mNormal);
    mNormal.Normalize();
    ComputeD();
}

void CPlane::CastRay(const CRay &_ray, CVec3f &_result) const
{
    float tmp1 = _ray.GetDirection().mX * mNormal.mX+
                 _ray.GetDirection().mY * mNormal.mY +
                 _ray.GetDirection().mZ * mNormal.mZ;

    float tmp = (mNormal.mX * _ray.GetPoint().mX +
                 mNormal.mY * _ray.GetPoint().mY +
                 mNormal.mZ * _ray.GetPoint().mZ +
                 mD) / tmp1;

    _result.mX = _ray.GetPoint().mX - _ray.GetDirection().mX * tmp;
    _result.mY = _ray.GetPoint().mY - _ray.GetDirection().mY * tmp;
    _result.mZ = _ray.GetPoint().mZ - _ray.GetDirection().mZ * tmp;
}

void CPlane::ComputeD()
{
    // Ax + Bx + Cx + D = 0
    mD = - mNormal.mX * mPoint.mX - mNormal.mY * mPoint.mY - mNormal.mZ * mPoint.mZ;
}

} // namespace drash
