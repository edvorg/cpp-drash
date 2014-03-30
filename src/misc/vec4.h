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

#ifndef DRASH_VEC4_H
#define DRASH_VEC4_H

#include "vec3.h"

namespace drash {

    template <typename T, const int DEF_VAL>
    class Vec4 : public Vec3<T, DEF_VAL> {
    public:
        Vec4() = default;
        Vec4(const Vec2<T, DEF_VAL>& _xy, const T& _z, const T& _w);
        Vec4(const Vec3<T, DEF_VAL>& _xyz, const T& _w);
        Vec4(const T& _x, const T& _y, const T& _z, const T& _w);

        /// setters. just syntax sugare

        Vec4& Set(const T& _x, const T& _y, const T& _z, const T& _w);

        /// conversion to CVec3

        inline const Vec3<T, DEF_VAL>& AsVec3() const;
        inline Vec3<T, DEF_VAL>& AsVec3();

        /// misc

        inline T LengthSquared() const;
        inline T Length() const;
        Vec4& Normalize();

        /// operators

        Vec4& operator=(const Vec4& _v);
        Vec4& operator+=(const Vec4& _v);
        Vec4& operator-=(const Vec4& _v);
        Vec4& operator*=(const Vec4& _v);
        Vec4& operator/=(const Vec4& _v);

        T w = static_cast<T>(DEF_VAL);

    protected:
    private:
    };

    /// ///////////////////// ///
    /// some predefined types ///

    typedef Vec4<float, 0> Vec4f;
    typedef Vec4<int, 0> Vec4i;
    typedef Vec4<unsigned int, 0> Vec4ui;

    /// some global functions

    template <typename T, const int DEF_VAL>
    Logger& operator<<(Logger& _logger, const Vec4<T, DEF_VAL>& _v) {
        _logger << '(' << _v.x << "; " << _v.y << "; " << _v.z << "; " << _v.w
                << ')';
        return _logger;
    }

    /// //////////////////// ///
    /// Vec4 implementation ///

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>::Vec4(const Vec2<T, DEF_VAL>& _xy, const T& _z,
                             const T& _w)
        : w(_w) {
        this->x = _xy.x;
        this->y = _xy.y;
        this->z = _z;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>::Vec4(const Vec3<T, DEF_VAL>& _xyz, const T& _w)
        : Vec3<T, DEF_VAL>(_xyz), w(_w) {}

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>::Vec4(const T& _x, const T& _y, const T& _z, const T& _w)
        : w(_w) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::Set(const T& _x, const T& _y,
                                              const T& _z, const T& _w) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = _w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline const Vec3<T, DEF_VAL>& Vec4<T, DEF_VAL>::AsVec3() const {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline Vec3<T, DEF_VAL>& Vec4<T, DEF_VAL>::AsVec3() {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec4<T, DEF_VAL>::LengthSquared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z +
               this->w * this->w;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec4<T, DEF_VAL>::Length() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z +
                    this->w * this->w);
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::Normalize() {
        T len = Vec4::Length();
        this->x /= len;
        this->y /= len;
        this->z /= len;
        this->w /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::operator=(const Vec4& _v) {
        this->x = _v.x;
        this->y = _v.y;
        this->z = _v.z;
        this->w = _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::operator+=(const Vec4& _v) {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        this->w += _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::operator-=(const Vec4& _v) {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        this->w -= _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::operator*=(const Vec4& _v) {
        this->x *= _v.x;
        this->y *= _v.y;
        this->z *= _v.z;
        this->w *= _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec4<T, DEF_VAL>& Vec4<T, DEF_VAL>::operator/=(const Vec4& _v) {
        this->x /= _v.x;
        this->y /= _v.y;
        this->z /= _v.z;
        this->w /= _v.w;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC4_H
