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

#ifndef DRASH_MATRIX4_H
#define DRASH_MATRIX4_H

#include "vec4.h"

namespace drash {

    class CMatrix4f {
      public:
        constexpr static const unsigned int rowsCount = 4;
        constexpr static const unsigned int columnsCount = 4;
        constexpr static const unsigned int elemsCount = 16;
        constexpr static const unsigned int i00 = 0;
        constexpr static const unsigned int i01 = 1;
        constexpr static const unsigned int i02 = 2;
        constexpr static const unsigned int i03 = 3;
        constexpr static const unsigned int i10 = 4;
        constexpr static const unsigned int i11 = 5;
        constexpr static const unsigned int i12 = 6;
        constexpr static const unsigned int i13 = 7;
        constexpr static const unsigned int i20 = 8;
        constexpr static const unsigned int i21 = 9;
        constexpr static const unsigned int i22 = 10;
        constexpr static const unsigned int i23 = 11;
        constexpr static const unsigned int i30 = 12;
        constexpr static const unsigned int i31 = 13;
        constexpr static const unsigned int i32 = 14;
        constexpr static const unsigned int i33 = 15;

        CMatrix4f() = default;
        CMatrix4f(const CMatrix4f& _m);

        CMatrix4f& Zero();
        CMatrix4f& Identity();
        CMatrix4f& Transpose();

        float data[elemsCount];

      protected:
      private:
    };

    CMatrix4f& MatrixMultiply(const CMatrix4f& _m1, const CMatrix4f& _m2,
                              CMatrix4f& _result);
    CVec4f& MatrixMultiply(const CMatrix4f& _m, const CVec4f& _v,
                           CVec4f& _result);
    CMatrix4f& MatrixScale(CMatrix4f& _m, const CVec3f _scale);
    CMatrix4f& MatrixRotationX(CMatrix4f& _m, float _angle);
    CMatrix4f& MatrixRotationY(CMatrix4f& _m, float _angle);
    CMatrix4f& MatrixRotationZ(CMatrix4f& _m, float _angle);
    CMatrix4f& MatrixTranslation(CMatrix4f& _m, const CVec3f& _translation);
    CMatrix4f& Matrix4Perspective(CMatrix4f& _m, float _fov, float _aspect,
                                  float _znear, float _zfar);

} // namespace drash

#endif // DRASH_MATRIX4_H
