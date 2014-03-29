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

namespace drash {

    extern const CPlane PlaneXY(CVec3f(0), CVec3f(0, 0, 1));
    extern const CPlane PlaneYZ(CVec3f(0), CVec3f(1, 0, 0));
    extern const CPlane PlaneXZ(CVec3f(0), CVec3f(0, 1, 0));

    CPlane::CPlane() {}

    CPlane::CPlane(const CPlane& _plane)
        : point(_plane.point), normal(_plane.normal), d(_plane.d) {}

    CPlane::CPlane(const CVec3f& _point, const CVec3f& _normal) {
        point = _point;
        normal = _normal;
        normal.Normalize();
        ComputeD();
    }

    void CPlane::SetPoint(const CVec3f& _point) {
        point = _point;
        ComputeD();
    }

    void CPlane::SetNormal(const CVec3f& _normal) {
        normal = _normal;
        normal.Normalize();
        ComputeD();
    }

    void CPlane::Set(const CVec3f& _p1, const CVec3f& _p2, const CVec3f& _p3) {
        point = _p1;
        CVec3f a(_p2);
        CVec3f b(_p3);
        a -= _p1;
        b -= _p1;
        Vec3Cross(a, b, normal);
        normal.Normalize();
        ComputeD();
    }

    void CPlane::CastRay(const CRay& _ray, CVec3f& _result) const {
        float tmp1 = _ray.GetDirection().x * normal.x +
                     _ray.GetDirection().y * normal.y +
                     _ray.GetDirection().z * normal.z;

        float tmp =
            (normal.x * _ray.GetPoint().x + normal.y * _ray.GetPoint().y +
             normal.z * _ray.GetPoint().z + d) /
            tmp1;

        _result.x = _ray.GetPoint().x - _ray.GetDirection().x * tmp;
        _result.y = _ray.GetPoint().y - _ray.GetDirection().y * tmp;
        _result.z = _ray.GetPoint().z - _ray.GetDirection().z * tmp;
    }

    void CPlane::ComputeD() {
        // Ax + Bx + Cx + D = 0
        d = -normal.x * point.x - normal.y * point.y - normal.z * point.z;
    }

} // namespace drash
