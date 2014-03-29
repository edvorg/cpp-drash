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
    class CVec4 : public CVec3<T, DEF_VAL> {
    public:
        CVec4() = default;
        CVec4(const CVec2<T, DEF_VAL>& _xy, const T& _z, const T& _w);
        CVec4(const CVec3<T, DEF_VAL>& _xyz, const T& _w);
        CVec4(const T& _x, const T& _y, const T& _z, const T& _w);

        /// setters. just syntax sugare

        CVec4& Set(const T& _x, const T& _y, const T& _z, const T& _w);

        /// conversion to CVec3

        inline const CVec3<T, DEF_VAL>& Vec3() const;
        inline CVec3<T, DEF_VAL>& Vec3();

        /// misc

        inline T LengthSquared() const;
        inline T Length() const;
        CVec4& Normalize();

        /// operators

        CVec4& operator=(const CVec4& _v);
        CVec4& operator+=(const CVec4& _v);
        CVec4& operator-=(const CVec4& _v);
        CVec4& operator*=(const CVec4& _v);
        CVec4& operator/=(const CVec4& _v);

        T w = static_cast<T>(DEF_VAL);

    protected:
    private:
    };

    /// ///////////////////// ///
    /// some predefined types ///

    typedef CVec4<float, 0> CVec4f;
    typedef CVec4<int, 0> CVec4i;
    typedef CVec4<unsigned int, 0> CVec4ui;

    /// some global functions

    template <typename T, const int DEF_VAL>
    CLogger& operator<<(CLogger& _logger, const CVec4<T, DEF_VAL>& _v) {
        _logger << '(' << _v.x << "; " << _v.y << "; " << _v.z << "; " << _v.w
                << ')';
        return _logger;
    }

    /// //////////////////// ///
    /// CVec4 implementation ///

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>::CVec4(const CVec2<T, DEF_VAL>& _xy, const T& _z,
                             const T& _w)
        : w(_w) {
        this->x = _xy.x;
        this->y = _xy.y;
        this->z = _z;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>::CVec4(const CVec3<T, DEF_VAL>& _xyz, const T& _w)
        : CVec3<T, DEF_VAL>(_xyz), w(_w) {}

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>::CVec4(const T& _x, const T& _y, const T& _z, const T& _w)
        : w(_w) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::Set(const T& _x, const T& _y,
                                              const T& _z, const T& _w) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = _w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline const CVec3<T, DEF_VAL>& CVec4<T, DEF_VAL>::Vec3() const {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline CVec3<T, DEF_VAL>& CVec4<T, DEF_VAL>::Vec3() {
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline T CVec4<T, DEF_VAL>::LengthSquared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z +
               this->w * this->w;
    }

    template <typename T, const int DEF_VAL>
    inline T CVec4<T, DEF_VAL>::Length() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z +
                    this->w * this->w);
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::Normalize() {
        T len = CVec4::Length();
        this->x /= len;
        this->y /= len;
        this->z /= len;
        this->w /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::operator=(const CVec4& _v) {
        this->x = _v.x;
        this->y = _v.y;
        this->z = _v.z;
        this->w = _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::operator+=(const CVec4& _v) {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        this->w += _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::operator-=(const CVec4& _v) {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        this->w -= _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::operator*=(const CVec4& _v) {
        this->x *= _v.x;
        this->y *= _v.y;
        this->z *= _v.z;
        this->w *= _v.w;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec4<T, DEF_VAL>& CVec4<T, DEF_VAL>::operator/=(const CVec4& _v) {
        this->x /= _v.x;
        this->y /= _v.y;
        this->z /= _v.z;
        this->w /= _v.w;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC4_H
