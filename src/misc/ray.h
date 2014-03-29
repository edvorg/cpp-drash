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

#ifndef DRASH_RAY_H
#define DRASH_RAY_H

#include "vec3.h"

namespace drash
{

class CRay
{
public:
    CRay();

    void SetPoint(const CVec3f &_point);
    inline const CVec3f &GetPoint() const;
    void SetDirection(const CVec3f &_direction);
    inline const CVec3f &GetDirection() const;

private:
    CVec3f mPoint;
    CVec3f mDirection;
};

inline const CVec3f &CRay::GetPoint() const
{
    return mPoint;
}

inline const CVec3f &CRay::GetDirection() const
{
    return mDirection;
}

} // namespace drash

#endif // DRASH_RAY_H
