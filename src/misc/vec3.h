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

        T mZ = static_cast<T>(DEF_VAL);

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
        return CVec3<T, DEF_VAL>(_v1.mX + _v2.mX, _v1.mY + _v2.mY,
                                 _v1.mZ + _v2.mZ);
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL> operator-(const CVec3<T, DEF_VAL>& _v1,
                                const CVec3<T, DEF_VAL>& _v2) {
        return CVec3<T, DEF_VAL>(_v1.mX - _v2.mX, _v1.mY - _v2.mY,
                                 _v1.mZ - _v2.mZ);
    }

    template <typename T, const int DEF_VAL>
    CLogger& operator<<(CLogger& _logger, const CVec3<T, DEF_VAL>& _v) {
        _logger << '(' << _v.mX << "; " << _v.mY << "; " << _v.mZ << ')';
        return _logger;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& Vec3Cross(const CVec3<T, DEF_VAL>& _v1,
                                 const CVec3<T, DEF_VAL>& _v2,
                                 CVec3<T, DEF_VAL>& _result) {
        _result.mX = _v1.mY * _v2.mZ - _v1.mZ * _v2.mY;
        _result.mY = _v1.mZ * _v2.mX - _v1.mX * _v2.mZ;
        _result.mZ = _v1.mX * _v2.mY - _v1.mY * _v2.mX;
        return _result;
    }

    /// //////////////////// ///
    /// CVec3 implementation ///

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const CVec3& _xyz)
        : CVec2<T, DEF_VAL>(_xyz.Vec2()), mZ(_xyz.mZ) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const CVec2<T, DEF_VAL>& _xy, const T& _z)
        : CVec2<T, DEF_VAL>(_xy), mZ(_z) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const T& _xyz)
        : CVec2<T, DEF_VAL>(_xyz), mZ(_xyz) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>::CVec3(const T& _x, const T& _y, const T& _z)
        : CVec2<T, DEF_VAL>(_x, _y), mZ(_z) {}

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Set(const CVec2<T, DEF_VAL>& _xy,
                                              const T& _z) {
        this->mX = _xy.mX;
        this->mY = _xy.mY;
        this->mZ = _z;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Set(const T& _x, const T& _y,
                                              const T& _z) {
        this->mX = _x;
        this->mY = _y;
        this->mZ = _z;
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
        return this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ;
    }

    template <typename T, const int DEF_VAL>
    inline T CVec3<T, DEF_VAL>::Length() const {
        return sqrt(this->mX * this->mX + this->mY * this->mY +
                    this->mZ * this->mZ);
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::Normalize() {
        T len = this->Length();
        this->mX /= len;
        this->mY /= len;
        this->mZ /= len;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator=(const CVec3& _v) {
        this->mX = _v.mX;
        this->mY = _v.mY;
        this->mZ = _v.mZ;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator+=(const CVec3& _v) {
        this->mX += _v.mX;
        this->mY += _v.mY;
        this->mZ += _v.mZ;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator-=(const CVec3& _v) {
        this->mX -= _v.mX;
        this->mY -= _v.mY;
        this->mZ -= _v.mZ;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator*=(const CVec3& _v) {
        this->mX *= _v.mX;
        this->mY *= _v.mY;
        this->mZ *= _v.mZ;
        return *this;
    }

    template <typename T, const int DEF_VAL>
    CVec3<T, DEF_VAL>& CVec3<T, DEF_VAL>::operator/=(const CVec3& _v) {
        this->mX /= _v.mX;
        this->mY /= _v.mY;
        this->mZ /= _v.mZ;
        return *this;
    }

} // namespace drash

#endif // DRASH_VEC3_H
