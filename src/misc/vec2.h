#ifndef DRASH_VEC2_H
#define DRASH_VEC2_H

#include <Box2D/Box2D.h>
#include "../diag/logger.h"

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

constexpr const unsigned int b2Vec2size = sizeof(b2Vec2);
constexpr const unsigned int CVec2fsize = sizeof(CVec2f);

#if b2Vec2size == CVec2fsize

inline const b2Vec2 &CVec2ToB2Vec2(const CVec2f &_v)
{
    return reinterpret_cast<const b2Vec2 &>(_v);
}

inline const CVec2f &B2Vec2ToCVec2(const b2Vec2 &_v)
{
    return reinterpret_cast<const CVec2f &>(_v);
}

#else

#error platform unsupported b2Vec2size != CVec2fsize

#endif

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
