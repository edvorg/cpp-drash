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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "vec2.h"
#include "math.h"

namespace drash
{

extern const float g_LayerWidth;

void DrawBodySide(const CVec2f &_v1,
              const CVec2f &_v2,
              float _z,
              float _depth,
              const b2Color &_diffuse );

void DrawBody(const CVec2f *_vertices,
              unsigned int _count,
              float _z,
              float _depth,
              const b2Color &_color);

void DrawCircle(float _rad,
                float _r, float _g, float _b, float _a);

}// namespace drash

#endif // GRAPHICS_H
