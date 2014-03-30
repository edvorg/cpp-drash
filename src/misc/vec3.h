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
    class Vec3 : public Vec2<T, DEF_VAL> {
    public:
        Vec3() = default;
        Vec3(const Vec3& _xyz);
        Vec3(const Vec2<T, DEF_VAL>& _xy, const T& _z);
        Vec3(const T& _xyz);
        Vec3(const T& _x, const T& _y, const T& _z);

        /// setters. just syntax sugare

        Vec3& Set(const Vec2<T, DEF_VAL>& _xy, const T& _z);
        Vec3& Set(const T& _x, const T& _y, const T& _z);

        /// conversion

        inline const Vec2<T, DEF_VAL>& AsVec2() const;
        inline Vec2<T, DEF_VAL>& AsVec2();

        /// misc

        inline T LengthSquared() const;
        inline T Length() const;
        Vec3& Normalize();

        /// operators

        Vec3& operator=(const Vec3& _v);
        Vec3& operator+=(const Vec3& _v);
        Vec3& operator-=(const Vec3& _v);
        Vec3& operator*=(const Vec3& _v);
        Vec3& operator/=(const Vec3& _v);

        T z = static_cast<T>(DEF_VAL);

    protected:
    private:
    };

    /// ///////////////////// ///
    /// some predefined types ///

    typedef Vec3<float, 0> Vec3f;
    typedef Vec3<int, 0> Vec3i;
    typedef Vec3<unsigned int, 0> Vec3ui;

    /// some global functions

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL> operator+(const Vec3<T, DEF_VAL>& _v1,
                                const Vec3<T, DEF_VAL>& _v2) {
        return Vec3<T, DEF_VAL>(_v1.x + _v2.x, _v1.y + _v2.y, _v1.z + _v2.z);
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL> operator-(const Vec3<T, DEF_VAL>& _v1,
                                const Vec3<T, DEF_VAL>& _v2) {
        return Vec3<T, DEF_VAL>(_v1.x - _v2.x, _v1.y - _v2.y, _v1.z - _v2.z);
    }

    template <typename T, const int DEF_VAL>
    Logger& operator<<(Logger& _logger, const Vec3<T, DEF_VAL>& _v) {
        _logger << '(' << _v.x << "; " << _v.y << "; " << _v.z << ')';
        return _logger;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3Cross(const Vec3<T, DEF_VAL>& _v1,
                                 const Vec3<T, DEF_VAL>& _v2,
                                 Vec3<T, DEF_VAL>& _result) {
        _result.x = _v1.y * _v2.z - _v1.z * _v2.y;
        _result.y = _v1.z * _v2.x - _v1.x * _v2.z;
        _result.z = _v1.x * _v2.y - _v1.y * _v2.x;
        return _result;
    }

    /// //////////////////// ///
    /// Vec3 implementation ///

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>::Vec3(const Vec3& _xyz)
        : Vec2<T, DEF_VAL>(_xyz), z(_xyz.z) {}

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>::Vec3(const Vec2<T, DEF_VAL>& _xy, const T& _z)
        : Vec2<T, DEF_VAL>(_xy), z(_z) {}

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>::Vec3(const T& _xyz)
        : Vec2<T, DEF_VAL>(_xyz), z(_xyz) {}

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>::Vec3(const T& _x, const T& _y, const T& _z)
        : Vec2<T, DEF_VAL>(_x, _y), z(_z) {}

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::Set(const Vec2<T, DEF_VAL>& _xy,
                                              const T& _z) {
        this->x = _xy.x;
        this->y = _xy.y;
        this->z = _z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::Set(const T& _x, const T& _y,
                                              const T& _z) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline const Vec2<T, DEF_VAL>& Vec3<T, DEF_VAL>::AsVec2() const {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline Vec2<T, DEF_VAL>& Vec3<T, DEF_VAL>::AsVec2() {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec3<T, DEF_VAL>::LengthSquared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec3<T, DEF_VAL>::Length() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::Normalize() {
        T len = this->Length();
        this->x /= len;
        this->y /= len;
        this->z /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::operator=(const Vec3& _v) {
        this->x = _v.x;
        this->y = _v.y;
        this->z = _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::operator+=(const Vec3& _v) {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::operator-=(const Vec3& _v) {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::operator*=(const Vec3& _v) {
        this->x *= _v.x;
        this->y *= _v.y;
        this->z *= _v.z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec3<T, DEF_VAL>& Vec3<T, DEF_VAL>::operator/=(const Vec3& _v) {
        this->x /= _v.x;
        this->y /= _v.y;
        this->z /= _v.z;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC3_H
