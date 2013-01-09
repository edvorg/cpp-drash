// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

namespace drash
{

template<typename T, const int DEF_VAL>
class CVec2
{
public:
    CVec2() = default;
    CVec2(const CVec2 &_xy);
    CVec2(const T &_xy);
    CVec2(const T &_x, const T &_y);

    /// setters. just syntax sugare

    CVec2 &Set(const T &_x, const T &_y);

    /// misc

    inline T LengthSquared() const;
    inline T Length() const;
    CVec2 &Normalize();

    /// operators

    CVec2 &operator =(const CVec2 &_v);
    CVec2 &operator +=(const CVec2 &_v);
    CVec2 &operator -=(const CVec2 &_v);
    CVec2 &operator *=(const CVec2 &_v);
    CVec2 &operator /=(const CVec2 &_v);

    T mX = static_cast<T>(DEF_VAL);
    T mY = static_cast<T>(DEF_VAL);

protected:
private:
};

/// ///////////////////// ///
/// some predefined types ///

typedef CVec2<float, 0> CVec2f;
typedef CVec2<int, 0> CVec2i;
typedef CVec2<unsigned int, 0> CVec2ui;

/// some global functions

const b2Vec2 &CVec2ToB2Vec2(const CVec2f &_v);
const CVec2f &B2Vec2ToCVec2(const b2Vec2 &_v);

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> operator +(const CVec2<T, DEF_VAL> &_v1, const CVec2<T, DEF_VAL> &_v2)
{
    return CVec2<T, DEF_VAL>(_v1.mX + _v2.mX, _v1.mY + _v2.mY);
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> operator -(const CVec2<T, DEF_VAL> &_v1, const CVec2<T, DEF_VAL> &_v2)
{
    return CVec2<T, DEF_VAL>(_v1.mX - _v2.mX, _v1.mY - _v2.mY);
}

template<typename T, const int DEF_VAL>
CLogger &operator<<(CLogger& _logger, const CVec2<T, DEF_VAL> &_v)
{
    _logger<<'('<<_v.mX<<"; "<<_v.mY<<')';
    return _logger;
}

/// //////////////////// ///
/// CVec2 implementation ///

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL>::CVec2(const CVec2 &_xy):
    mX(_xy.mX),
    mY(_xy.mY)
{
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL>::CVec2(const T &_xy):
    mX(_xy),
    mY(_xy)
{
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL>::CVec2(const T &_x, const T &_y):
    mX(_x),
    mY(_y)
{
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::Set(const T &_x, const T &_y)
{
    mX = _x;
    mY = _y;
    return *this;
}

template<typename T, const int DEF_VAL>
inline T CVec2<T, DEF_VAL>::LengthSquared() const
{
    return mX * mX + mY * mY;
}

template<typename T, const int DEF_VAL>
inline T CVec2<T, DEF_VAL>::Length() const
{
    return sqrt(mX * mX + mY * mY);
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::Normalize()
{
    T len = this->Length();
    mX /= len;
    mY /= len;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::operator =(const CVec2 &_v)
{
    mX = _v.mX;
    mY = _v.mY;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::operator +=(const CVec2 &_v)
{
    mX += _v.mX;
    mY += _v.mY;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::operator -=(const CVec2 &_v)
{
    mX -= _v.mX;
    mY -= _v.mY;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::operator *=(const CVec2 &_v)
{
    mX *= _v.mX;
    mY *= _v.mY;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec2<T, DEF_VAL> &CVec2<T, DEF_VAL>::operator /=(const CVec2 &_v)
{
    mX /= _v.mX;
    mY /= _v.mY;
    return *this;
}

} // namespace drash

#endif // DRASH_VEC2_H
