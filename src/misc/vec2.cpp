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

#include <Box2D/Box2D.h>
#include "vec2.h"

namespace drash {

    constexpr const unsigned int b2Vec2size = sizeof(b2Vec2);
    constexpr const unsigned int Vec2fsize = sizeof(Vec2f);

    static const b2Vec2 v1;
    static const Vec2f v2(0, 0);

    constexpr const void* p1 = &v1;
    constexpr const void* p2 = &v1.x;
    constexpr const void* p3 = &v2;
    constexpr const void* p4 = &v2.x;

#if (b2Vec2size == Vec2fsize && p1 == p2 && p3 == p4)

    const b2Vec2& Vec2ToB2Vec2(const Vec2f& _v) {
        return reinterpret_cast<const b2Vec2&>(_v);
    }

    const Vec2f& B2Vec2ToCVec2(const b2Vec2& _v) {
        return reinterpret_cast<const Vec2f&>(_v);
    }

#else

#error platform unsupported b2Vec2size != Vec2fsize

#endif

} // namespace drash
