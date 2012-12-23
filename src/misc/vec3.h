#ifndef DRASH_VEC3_H
#define DRASH_VEC3_H

#include "vec2.h"

namespace drash
{

template<typename T, const int DEF_VAL>
class CVec3
{
public:
    CVec3() = default;
    CVec3(const CVec2 &_xy, const T &_z);
    CVec3(const CVec3 &_xyz);
    CVec3(const T &_xyz);
    CVec3(const T &_x, const T &_y, const T &_z);

    /// setters. just syntax sugare

    CVec3 &Set(const CVec2 &_xy, const T &_z);
    CVec3 &Set(const T &_x, const T &_y, const T &_z);

    /// misc

    inline T LengthSquared() const;
    inline T Length() const;
    CVec3 &Normalize();
    inline CVec2 Vec2() const;

    /// operators

    CVec3 &operator =(const CVec3 &_v);
    CVec3 &operator +=(const CVec3 &_v);
    CVec3 &operator -=(const CVec3 &_v);
    CVec3 &operator *=(const CVec3 &_v);
    CVec3 &operator /=(const CVec3 &_v);

    T mX = static_cast<T>(DEF_VAL);
    T mY = static_cast<T>(DEF_VAL);
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

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> operator +(const CVec3<T, DEF_VAL> &_v1, const CVec3<T, DEF_VAL> &_v2)
{
    return CVec3<T, DEF_VAL>(_v1.mX + _v2.mX, _v1.mY + _v2.mY, _v1.mZ + _v2.mZ);
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> operator -(const CVec3<T, DEF_VAL> &_v1, const CVec3<T, DEF_VAL> &_v2)
{
    return CVec3<T, DEF_VAL>(_v1.mX - _v2.mX, _v1.mY - _v2.mY, _v1.mZ - _v2.mZ);
}

template<typename T, const int DEF_VAL>
CLogger &operator<<(CLogger& _logger, const CVec3<T, DEF_VAL> &_v)
{
    _logger<<'('<<_v.mX<<"; "<<_v.mY<<"; "<<_v.mZ<<')';
    return _logger;
}

/// //////////////////// ///
/// CVec3 implementation ///

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL>::CVec3(const CVec2 &_xy, const T &_z):
    mX(_xy.x),
    mY(_xy.y),
    mZ(_z)
{
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL>::CVec3(const CVec3 &_xyz):
    mX(_xyz.mX),
    mY(_xyz.mY),
    mZ(_xyz.mZ)
{
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL>::CVec3(const T &_xyz):
    mX(_xyz),
    mY(_xyz),
    mZ(_xyz)
{
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL>::CVec3(const T &_x, const T &_y, const T &_z):
    mX(_x),
    mY(_y),
    mZ(_z)
{
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::Set(const CVec2 &_xy, const T &_z)
{
    mX = _xy.x;
    mY = _xy.y;
    mZ = _z;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::Set(const T &_x, const T &_y, const T &_z)
{
    mX = _x;
    mY = _y;
    mZ = _z;
    return *this;
}

template<typename T, const int DEF_VAL>
inline T CVec3<T, DEF_VAL>::LengthSquared() const
{
    return mX * mX + mY * mY + mZ * mZ;
}

template<typename T, const int DEF_VAL>
inline T CVec3<T, DEF_VAL>::Length() const
{
    return sqrt(mX * mX + mY * mY + mZ * mZ);
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::Normalize()
{
    T len = this->Length();
    mX /= len;
    mY /= len;
    mZ /= len;
    return *this;
}

template<typename T, const int DEF_VAL>
inline CVec2 CVec3<T, DEF_VAL>::Vec2() const
{
    return CVec2(mX, mY);
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::operator =(const CVec3 &_v)
{
    mX = _v.mX;
    mY = _v.mY;
    mZ = _v.mZ;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::operator +=(const CVec3 &_v)
{
    mX += _v.mX;
    mY += _v.mY;
    mZ += _v.mZ;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::operator -=(const CVec3 &_v)
{
    mX -= _v.mX;
    mY -= _v.mY;
    mZ -= _v.mZ;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::operator *=(const CVec3 &_v)
{
    mX *= _v.mX;
    mY *= _v.mY;
    mZ *= _v.mZ;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::operator /=(const CVec3 &_v)
{
    mX /= _v.mX;
    mY /= _v.mY;
    mZ /= _v.mZ;
    return *this;
}

} // namespace drash

#endif // DRASH_VEC3_H
