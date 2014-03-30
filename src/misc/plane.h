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

#ifndef DRASH_PLANE_H
#define DRASH_PLANE_H

#include "vec3.h"

namespace drash {

    class Ray;

    class Plane {
    public:
        Plane();
        Plane(const Plane& _plane);
        Plane(const Vec3f& _point, const Vec3f& _normal);

        void SetPoint(const Vec3f& _point);
        inline const Vec3f& GetPoint() const;
        void SetNormal(const Vec3f& _normal);
        inline const Vec3f& GetNormal() const;

        void Set(const Vec3f& _p1, const Vec3f& _p2, const Vec3f& _p3);

        void CastRay(const Ray& _ray, Vec3f& _result) const;

    private:
        void ComputeD();

        Vec3f point;
        Vec3f normal;
        float d = 0;
    };

    extern const Plane PlaneXY;
    extern const Plane PlaneYZ;
    extern const Plane PlaneXZ;

    inline const Vec3f& Plane::GetPoint() const { return point; }

    inline const Vec3f& Plane::GetNormal() const { return normal; }

} // namespace drash

#endif // DRASH_PLANE_H
