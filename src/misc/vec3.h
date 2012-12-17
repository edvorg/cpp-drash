#ifndef DRASH_VEC3_H
#define DRASH_VEC3_H

namespace drash
{

template<typename T, const int DEF_VAL>
class CVec3
{
public:
    CVec3() = default;
    CVec3(const CVec3 &_src);
    CVec3(const T &_xyz);
    CVec3(const T &_x, const T &_y, const T &_z);

    CVec3 &Set(const T &_x, const T &_y, const T &_z);

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

/// //////////////////// ///
/// CVec3 implementation ///

template<typename T, const int DEF_VAL>
CVec3<T, DEF_VAL>::CVec3(const CVec3 &_src):
    mX(_src.mX),
    mY(_src.mY),
    mZ(_src.mZ)
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
CVec3<T, DEF_VAL> &CVec3<T, DEF_VAL>::Set(const T &_x, const T &_y, const T &_z)
{
    mX = _x;
    mY = _y;
    mZ = _z;
    return *this;
}

} // namespace drash

#endif // DRASH_VEC3_H
