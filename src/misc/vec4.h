#ifndef DRASH_VEC4_H
#define DRASH_VEC4_H

#include "vec3.h"

namespace drash
{

template<typename T, const int DEF_VAL>
class CVec4 : public CVec3<T, DEF_VAL>
{
public:
    CVec4() = default;

    /// setters. just syntax sugare

    CVec4 &Set(const T &_x, const T &_y, const T &_z, const T &_w);

    /// conversion to CVec3

    inline const CVec3<T, DEF_VAL> &Vec3() const;
    inline CVec3<T, DEF_VAL> &Vec3();

    /// misc

    inline T LengthSquared() const;
    inline T Length() const;
    CVec4 &Normalize();

    /// operators

    CVec4 &operator =(const CVec4 &_v);
    CVec4 &operator +=(const CVec4 &_v);
    CVec4 &operator -=(const CVec4 &_v);
    CVec4 &operator *=(const CVec4 &_v);
    CVec4 &operator /=(const CVec4 &_v);

    T mW = static_cast<T>(DEF_VAL);
protected:
private:
};

/// ///////////////////// ///
/// some predefined types ///

typedef CVec4<float, 0> CVec4f;
typedef CVec4<int, 0> CVec4i;
typedef CVec4<unsigned int, 0> CVec4ui;

/// some global functions

template<typename T, const int DEF_VAL>
CLogger &operator<<(CLogger& _logger, const CVec4<T, DEF_VAL> &_v)
{
    _logger<<'('<<_v.mX<<"; "<<_v.mY<<"; "<<_v.mZ<<"; "<<_v.mW<<')';
    return _logger;
}

/// //////////////////// ///
/// CVec4 implementation ///

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::Set(const T &_x,
                                          const T &_y,
                                          const T &_z,
                                          const T &_w)
{
    this->mX = _x;
    this->mY = _y;
    this->mZ = _z;
    this->mW = _w;
}

template<typename T, const int DEF_VAL>
inline const CVec3<T, DEF_VAL> &CVec4<T, DEF_VAL>::Vec3() const
{
    return *this;
}

template<typename T, const int DEF_VAL>
inline CVec3<T, DEF_VAL> &CVec4<T, DEF_VAL>::Vec3()
{
    return *this;
}

template<typename T, const int DEF_VAL>
inline T CVec4<T, DEF_VAL>::LengthSquared() const
{
    return this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ + this->mW * this->mW;
}

template<typename T, const int DEF_VAL>
inline T CVec4<T, DEF_VAL>::Length() const
{
    return sqrt(this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ + this->mW * this->mW);
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::Normalize()
{
    T len = CVec4::Length();
    this->mX /= len;
    this->mY /= len;
    this->mZ /= len;
    this->mW /= len;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::operator =(const CVec4 &_v)
{
    this->mX = _v.mX;
    this->mY = _v.mY;
    this->mZ = _v.mZ;
    this->mW = _v.mW;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::operator +=(const CVec4 &_v)
{
    this->mX += _v.mX;
    this->mY += _v.mY;
    this->mZ += _v.mZ;
    this->mW += _v.mW;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::operator -=(const CVec4 &_v)
{
    this->mX -= _v.mX;
    this->mY -= _v.mY;
    this->mZ -= _v.mZ;
    this->mW -= _v.mW;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::operator *=(const CVec4 &_v)
{
    this->mX *= _v.mX;
    this->mY *= _v.mY;
    this->mZ *= _v.mZ;
    this->mW *= _v.mW;
    return *this;
}

template<typename T, const int DEF_VAL>
CVec4<T, DEF_VAL> &CVec4<T, DEF_VAL>::operator /=(const CVec4 &_v)
{
    this->mX /= _v.mX;
    this->mY /= _v.mY;
    this->mZ /= _v.mZ;
    this->mW /= _v.mW;
    return *this;
}

} // namespace drash

#endif // DRASH_VEC4_H
