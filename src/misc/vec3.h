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

#ifndef DRASH_VEC3_H
#define DRASH_VEC3_H

#include "vec2.h"

namespace drash {

    template <typename T, const int DEF_VAL>
    class CVec3 : public CVec2<T, DEF_VAL> {
    public:
        CVec3() = default;
        CVec3(const CVec3& _xyz);
        CVec3(const CVec2<T, DEF_VAL>& _xy, const T& _z);
        CVec3(const T& _xyz);
        CVec3(const T& _x, const T& _y, const T& _z);

        /// setters. just syntax sugare

        CVec3& Set(const CVec2<T, DEF_VAL>& _xy, const T& _z);
        CVec3& Set(const T& _x, const T& _y, const T& _z);

        /// conversion

        inline const CVec2<T, DEF_VAL>& Vec2() const;
        inline CVec2<T, DEF_VAL>& Vec2();

        /// misc

        inline T LengthSquared() const;
        inline T Length() const;
        CVec3& Normalize();

        /// operators

        CVec3& operator=(const CVec3& _v);
        CVec3& operator+=(const CVec3& _v);
        CVec3& operator-=(const CVec3& _v);
        CVec3& operator*=(const CVec3& _v);
        CVec3& operator/=(const CVec3& _v);

        T z = static_cast<T>(DEF_VAL);

    protected:
    private:
    };

    /// ///////////////////// ///
    /// some predefined types ///

    typedef CVec3<float, 0> CVec3f;
    typedef CVec3<int, 0> CVec3i;
    typedef CVec3<unsigned int, 0> CVec3ui;

    /// some global functions

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL> operator+(const CVec3<T, DEF_VAL>& _v1,
                                const CVec3<T, DEF_VAL>& _v2) {
        return CVec3<T, DEF_VAL>(_v1.x + _v2.x, _v1.y + _v2.y, _v1.z + _v2.z);
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL> operator-(const CVec3<T, DEF_VAL>& _v1,
                                const CVec3<T, DEF_VAL>& _v2) {
        return CVec3<T, DEF_VAL>(_v1.x - _v2.x, _v1.y - _v2.y, _v1.z - _v2.z);
    }

    template <typename T, const int DEF_VAL>
    CLogger& operator<<(CLogger& _logger, const CVec3<T, DEF_VAL>& _v) {
        _logger << '(' << _v.x << "; " << _v.y << "; " << _v.z << ')';
        return _logger;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& Vec3Cross(const CVec3<T, DEF_VAL>& _v1,
                                 const CVec3<T, DEF_VAL>& _v2,
                                 CVec3<T, DEF_VAL>& _result) {
        _result.x = _v1.y * _v2.z - _v1.z * _v2.y;
        _result.y = _v1.z * _v2.x - _v1.x * _v2.z;
        _result.z = _v1.x * _v2.y - _v1.y * _v2.x;
        return _result;
    }

    /// //////////////////// ///
    /// CVec3 implementation ///

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const CVec3& _xyz)
        : CVec2<T, DEF_VAL>(_xyz.Vec2()), z(_xyz.z) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const CVec2<T, DEF_VAL>& _xy, const T& _z)
        : CVec2<T, DEF_VAL>(_xy), z(_z) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const T& _xyz)
        : CVec2<T, DEF_VAL>(_xyz), z(_xyz) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const T& _x, const T& _y, const T& _z)
        : CVec2<T, DEF_VAL>(_x, _y), z(_z) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Set(const CVec2<T, DEF_VAL>& _xy,
                                              const T& _z) {
        this->x = _xy.x;
        this->y = _xy.y;
        this->z = _z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Set(const T& _x, const T& _y,
                                              const T& _z) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline const CVec2<T, DEF_VAL>& CVec3<T, DEF_VAL>::Vec2() const {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline CVec2<T, DEF_VAL>& CVec3<T, DEF_VAL>::Vec2() {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline T CVec3<T, DEF_VAL>::LengthSquared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    template <typename T, const int DEF_VAL>
    inline T CVec3<T, DEF_VAL>::Length() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Normalize() {
        T len = this->Length();
        this->x /= len;
        this->y /= len;
        this->z /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator=(const CVec3& _v) {
        this->x = _v.x;
        this->y = _v.y;
        this->z = _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator+=(const CVec3& _v) {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator-=(const CVec3& _v) {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator*=(const CVec3& _v) {
        this->x *= _v.x;
        this->y *= _v.y;
        this->z *= _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator/=(const CVec3& _v) {
        this->x /= _v.x;
        this->y /= _v.y;
        this->z /= _v.z;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC3_H
