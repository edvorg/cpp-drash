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

#include "matrix4.h"
#include <cstring>
#include <cmath>

namespace drash {

    CMatrix4f::CMatrix4f(const CMatrix4f& _m) {
        memcpy(data, _m.data, sizeof(float) * elemsCount);
    }

    CMatrix4f& CMatrix4f::Zero() {
        memset(data, 0, sizeof(float) * elemsCount);
        return *this;
    }

    CMatrix4f& CMatrix4f::Identity() {
        Zero();
        data[i00] = data[i11] = data[i22] = data[i33] = 1;
        return *this;
    }

    CMatrix4f& CMatrix4f::Transpose() {
        float tmp = data[i01];
        data[i01] = data[i10];
        data[i10] = tmp;

        tmp = data[i02];
        data[i02] = data[i20];
        data[i20] = tmp;

        tmp = data[i03];
        data[i03] = data[i30];
        data[i30] = tmp;

        tmp = data[i12];
        data[i12] = data[i21];        
        data[i21] = tmp;

        tmp = data[i13];
        data[i13] = data[i31];
        data[i31] = tmp;

        tmp = data[i23];
        data[i23] = data[i32];
        data[i32] = tmp;

        return *this;
    }

    CMatrix4f& MatrixMultiply(const CMatrix4f& _m1, const CMatrix4f& _m2,
                              CMatrix4f& _result) {
        _result.data[_result.i00] = _m1.data[_m1.i00] * _m2.data[_m2.i00] +
            _m1.data[_m1.i01] * _m2.data[_m2.i10] +
            _m1.data[_m1.i02] * _m2.data[_m2.i20] +
            _m1.data[_m1.i03] * _m2.data[_m2.i30];

        _result.data[_result.i01] = _m1.data[_m1.i00] * _m2.data[_m2.i01] +
            _m1.data[_m1.i01] * _m2.data[_m2.i11] +
            _m1.data[_m1.i02] * _m2.data[_m2.i21] +
            _m1.data[_m1.i03] * _m2.data[_m2.i31];

        _result.data[_result.i02] = _m1.data[_m1.i00] * _m2.data[_m2.i02] +
            _m1.data[_m1.i01] * _m2.data[_m2.i12] +
            _m1.data[_m1.i02] * _m2.data[_m2.i22] +
            _m1.data[_m1.i03] * _m2.data[_m2.i32];

        _result.data[_result.i03] = _m1.data[_m1.i00] * _m2.data[_m2.i03] +
            _m1.data[_m1.i01] * _m2.data[_m2.i13] +
            _m1.data[_m1.i02] * _m2.data[_m2.i23] +
            _m1.data[_m1.i03] * _m2.data[_m2.i33];

        _result.data[_result.i10] = _m1.data[_m1.i10] * _m2.data[_m2.i00] +
            _m1.data[_m1.i11] * _m2.data[_m2.i10] +
            _m1.data[_m1.i12] * _m2.data[_m2.i20] +
            _m1.data[_m1.i13] * _m2.data[_m2.i30];

        _result.data[_result.i11] = _m1.data[_m1.i10] * _m2.data[_m2.i01] +
            _m1.data[_m1.i11] * _m2.data[_m2.i11] +
            _m1.data[_m1.i12] * _m2.data[_m2.i21] +
            _m1.data[_m1.i13] * _m2.data[_m2.i31];

        _result.data[_result.i12] = _m1.data[_m1.i10] * _m2.data[_m2.i02] +
            _m1.data[_m1.i11] * _m2.data[_m2.i12] +
            _m1.data[_m1.i12] * _m2.data[_m2.i22] +
            _m1.data[_m1.i13] * _m2.data[_m2.i32];

        _result.data[_result.i13] = _m1.data[_m1.i10] * _m2.data[_m2.i03] +
            _m1.data[_m1.i11] * _m2.data[_m2.i13] +
            _m1.data[_m1.i12] * _m2.data[_m2.i23] +
            _m1.data[_m1.i13] * _m2.data[_m2.i33];

        _result.data[_result.i20] = _m1.data[_m1.i20] * _m2.data[_m2.i00] +
            _m1.data[_m1.i21] * _m2.data[_m2.i10] +
            _m1.data[_m1.i22] * _m2.data[_m2.i20] +
            _m1.data[_m1.i23] * _m2.data[_m2.i30];

        _result.data[_result.i21] = _m1.data[_m1.i20] * _m2.data[_m2.i01] +
            _m1.data[_m1.i21] * _m2.data[_m2.i11] +
            _m1.data[_m1.i22] * _m2.data[_m2.i21] +
            _m1.data[_m1.i23] * _m2.data[_m2.i31];

        _result.data[_result.i22] = _m1.data[_m1.i20] * _m2.data[_m2.i02] +
            _m1.data[_m1.i21] * _m2.data[_m2.i12] +
            _m1.data[_m1.i22] * _m2.data[_m2.i22] +
            _m1.data[_m1.i23] * _m2.data[_m2.i32];

        _result.data[_result.i23] = _m1.data[_m1.i20] * _m2.data[_m2.i03] +
            _m1.data[_m1.i21] * _m2.data[_m2.i13] +
            _m1.data[_m1.i22] * _m2.data[_m2.i23] +
            _m1.data[_m1.i23] * _m2.data[_m2.i33];

        _result.data[_result.i30] = _m1.data[_m1.i30] * _m2.data[_m2.i00] +
            _m1.data[_m1.i31] * _m2.data[_m2.i10] +
            _m1.data[_m1.i32] * _m2.data[_m2.i20] +
            _m1.data[_m1.i33] * _m2.data[_m2.i30];

        _result.data[_result.i31] = _m1.data[_m1.i30] * _m2.data[_m2.i01] +
            _m1.data[_m1.i31] * _m2.data[_m2.i11] +
            _m1.data[_m1.i32] * _m2.data[_m2.i21] +
            _m1.data[_m1.i33] * _m2.data[_m2.i31];

        _result.data[_result.i32] = _m1.data[_m1.i30] * _m2.data[_m2.i02] +
            _m1.data[_m1.i31] * _m2.data[_m2.i12] +
            _m1.data[_m1.i32] * _m2.data[_m2.i22] +
            _m1.data[_m1.i33] * _m2.data[_m2.i32];

        _result.data[_result.i33] = _m1.data[_m1.i30] * _m2.data[_m2.i03] +
            _m1.data[_m1.i31] * _m2.data[_m2.i13] +
            _m1.data[_m1.i32] * _m2.data[_m2.i23] +
            _m1.data[_m1.i33] * _m2.data[_m2.i33];

        return _result;
    }

    CVec4f& MatrixMultiply(const CMatrix4f& _m, const CVec4f& _v,
                           CVec4f& _result) {
        _result.x = _v.x * _m.data[_m.i00] + _v.y * _m.data[_m.i01] +
            _v.z * _m.data[_m.i02] + _v.w * _m.data[_m.i03];

        _result.y = _v.x * _m.data[_m.i10] + _v.y * _m.data[_m.i11] +
            _v.z * _m.data[_m.i12] + _v.w * _m.data[_m.i13];

        _result.z = _v.x * _m.data[_m.i20] + _v.y * _m.data[_m.i21] +
            _v.z * _m.data[_m.i22] + _v.w * _m.data[_m.i23];

        _result.w = _v.x * _m.data[_m.i30] + _v.y * _m.data[_m.i31] +
            _v.z * _m.data[_m.i32] + _v.w * _m.data[_m.i33];

        return _result;
    }

    CMatrix4f& MatrixScale(CMatrix4f& _m, const CVec3f _scale) {
        _m.Identity();
        _m.data[_m.i00] = _scale.x;
        _m.data[_m.i11] = _scale.y;
        _m.data[_m.i22] = _scale.z;

        return _m;
    }

    CMatrix4f& MatrixRotationX(CMatrix4f& _m, float _angle) {
        float c = cos(_angle);
        float s = sin(_angle);

        _m.Identity();
        _m.data[_m.i11] = _m.data[_m.i22] = c;
        _m.data[_m.i12] = -s;
        _m.data[_m.i21] = s;

        return _m;
    }

    CMatrix4f& MatrixRotationY(CMatrix4f& _m, float _angle) {
        float c = cos(_angle);
        float s = sin(_angle);

        _m.Identity();
        _m.data[_m.i00] = _m.data[_m.i22] = c;
        _m.data[_m.i02] = s;
        _m.data[_m.i20] = -s;

        return _m;
    }

    CMatrix4f& MatrixRotationZ(CMatrix4f& _m, float _angle) {
        float c = cos(_angle);
        float s = sin(_angle);

        _m.Identity();
        _m.data[_m.i00] = _m.data[_m.i11] = c;
        _m.data[_m.i01] = -s;
        _m.data[_m.i10] = s;

        return _m;
    }

    CMatrix4f& MatrixTranslation(CMatrix4f& _m, const CVec3f& _translation) {
        _m.Identity();

        _m.data[_m.i03] = _translation.x;
        _m.data[_m.i13] = _translation.y;
        _m.data[_m.i23] = _translation.z;

        return _m;
    }

    CMatrix4f& Matrix4Perspective(CMatrix4f& _m, float _fov, float _aspect,
                                  float _znear, float _zfar) {
        _m.Identity();

        float f = 1.0f / tan(_fov / 2.0f);
        float a = (_zfar + _znear) / (_znear - _zfar);
        float b = (2 * _zfar * _znear) / (_znear - _zfar);

        _m.data[_m.i00] = f / _aspect;
        _m.data[_m.i11] = f;
        _m.data[_m.i22] = a;
        _m.data[_m.i33] = 0;

        _m.data[_m.i23] = b;
        _m.data[_m.i32] = -1;

        return _m;
    }

} // namespace drash
