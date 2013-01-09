// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

namespace drash
{

class CRay;

class CPlane
{
public:
    CPlane();
    CPlane(const CPlane &_plane);
    CPlane(const CVec3f &_point, const CVec3f &_normal);

    void SetPoint(const CVec3f &_point);
    inline const CVec3f &GetPoint() const;
    void SetNormal(const CVec3f &_normal);
    inline const CVec3f &GetNormal() const;

    void Set(const CVec3f &_p1, const CVec3f &_p2, const CVec3f &_p3);

    void CastRay(const CRay &_ray, CVec3f &_result) const;

private:
    void ComputeD();

    CVec3f mPoint;
    CVec3f mNormal;
    float mD = 0;
};

extern const CPlane PlaneXY;
extern const CPlane PlaneYZ;
extern const CPlane PlaneXZ;

inline const CVec3f &CPlane::GetPoint() const
{
    return mPoint;
}

inline const CVec3f &CPlane::GetNormal() const
{
    return mNormal;
}

} // namespace drash

#endif // DRASH_PLANE_H
