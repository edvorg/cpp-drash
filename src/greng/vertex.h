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

#ifndef GRENG_VERTEX_H
#define GRENG_VERTEX_H

#include "../misc/vec3.h"
#include "../misc/color4.h"

namespace greng
{

class CVertex
{
public:
    CVertex() = default;

    drash::CVec3f mPos = drash::CVec3f(0);
    drash::CVec2f mUV = drash::CVec2f(0);
    drash::CVec3f mNormal = drash::CVec3f(0);
    drash::CColor4f mColor = drash::CColor4f(1);
    drash::CVec3f mTangent = drash::CVec3f(1, 0, 0);
    drash::CVec3f mBinormal = drash::CVec3f(0, 1, 0);

protected:
private:
};

} // namespace greng

#endif // GRENG_VERTEX_H
