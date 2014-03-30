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

#ifndef DRASH_VEC2_H
#define DRASH_VEC2_H

#include "../diag/logger.h"
#include <cmath>

struct b2Vec2;

namespace drash {

    template <typename T, const int DEF_VAL> class Vec2 {
    public:
        Vec2() = default;
        Vec2(const Vec2& _xy);
        Vec2(const T& _xy);
        Vec2(const T& _x, const T& _y);

        /// setters. just syntax sugare

        Vec2& Set(const T& _x, const T& _y);

        /// misc

        inline T LengthSquared() const;
        inline T Length() const;
        Vec2& Normalize();

        /// operators

        Vec2& operator=(const Vec2& _v);
        Vec2& operator+=(const Vec2& _v);
        Vec2& operator-=(const Vec2& _v);
        Vec2& operator*=(const Vec2& _v);
        Vec2& operator/=(const Vec2& _v);

        T x = static_cast<T>(DEF_VAL);
        T y = static_cast<T>(DEF_VAL);

    protected:
    private:
    };

    /// ///////////////////// ///
    /// some predefined types ///

    typedef Vec2<float, 0> Vec2f;
    typedef Vec2<int, 0> Vec2i;
    typedef Vec2<unsigned int, 0> Vec2ui;

    /// some global functions

    const b2Vec2& Vec2ToB2Vec2(const Vec2f& _v);
    const Vec2f& B2Vec2ToCVec2(const b2Vec2& _v);

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL> operator+(const Vec2<T, DEF_VAL>& _v1,
                               const Vec2<T, DEF_VAL>& _v2) {
        return Vec2<T, DEF_VAL>(_v1.x + _v2.x, _v1.y + _v2.y);
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL> operator-(const Vec2<T, DEF_VAL>& _v1,
                               const Vec2<T, DEF_VAL>& _v2) {
        return Vec2<T, DEF_VAL>(_v1.x - _v2.x, _v1.y - _v2.y);
    }

    template <typename T, const int DEF_VAL>
    Logger& operator<<(Logger& _logger, const Vec2<T, DEF_VAL>& _v) {
        _logger << '(' << _v.x << "; " << _v.y << ')';
        return _logger;
    }

    /// //////////////////// ///
    /// Vec2 implementation ///

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>::Vec2(const Vec2& _xy)
        : x(_xy.x), y(_xy.y) {}

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>::Vec2(const T& _xy)
        : x(_xy), y(_xy) {}

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>::Vec2(const T& _x, const T& _y)
        : x(_x), y(_y) {}

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::Set(const T& _x, const T& _y) {
        x = _x;
        y = _y;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec2<T, DEF_VAL>::LengthSquared() const {
        return x * x + y * y;
    }

    template <typename T, const int DEF_VAL>
    inline T Vec2<T, DEF_VAL>::Length() const {
        return sqrt(x * x + y * y);
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::Normalize() {
        T len = this->Length();
        x /= len;
        y /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::operator=(const Vec2& _v) {
        x = _v.x;
        y = _v.y;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::operator+=(const Vec2& _v) {
        x += _v.x;
        y += _v.y;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::operator-=(const Vec2& _v) {
        x -= _v.x;
        y -= _v.y;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::operator*=(const Vec2& _v) {
        x *= _v.x;
        y *= _v.y;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    Vec2<T, DEF_VAL>& Vec2<T, DEF_VAL>::operator/=(const Vec2& _v) {
        x /= _v.x;
        y /= _v.y;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC2_H
