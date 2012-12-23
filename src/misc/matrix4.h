#ifndef DRASH_MATRIX4_H
#define DRASH_MATRIX4_H

#include "vec4.h"

namespace drash
{

class CMatrix4f
{
public:
    constexpr static const unsigned int mRowsCount = 4;
    constexpr static const unsigned int mColumnsCount = 4;
    constexpr static const unsigned int mElemsCount = 16;
    constexpr static const unsigned int m00 = 0;
    constexpr static const unsigned int m01 = 1;
    constexpr static const unsigned int m02 = 2;
    constexpr static const unsigned int m03 = 3;
    constexpr static const unsigned int m10 = 4;
    constexpr static const unsigned int m11 = 5;
    constexpr static const unsigned int m12 = 6;
    constexpr static const unsigned int m13 = 7;
    constexpr static const unsigned int m20 = 8;
    constexpr static const unsigned int m21 = 9;
    constexpr static const unsigned int m22 = 10;
    constexpr static const unsigned int m23 = 11;
    constexpr static const unsigned int m30 = 12;
    constexpr static const unsigned int m31 = 13;
    constexpr static const unsigned int m32 = 14;
    constexpr static const unsigned int m33 = 15;

    CMatrix4f() = default;

    CMatrix4f &Zero();
    CMatrix4f &Identity();

    float mData[mElemsCount];
protected:
private:
};

CMatrix4f &MatrixMultiply(const CMatrix4f &_m1, const CMatrix4f &_m2, CMatrix4f &_result);
CVec4f &MatrixMultiply(const CMatrix4f &_m, const CVec4f &_v, CVec4f &_result);
CVec4f &MatrixMultiply(const CVec4f &_v, const CMatrix4f &_m, CVec4f &_result);
CMatrix4f &MatrixRotationX(CMatrix4f &_m, float _angle);
CMatrix4f &MatrixRotationY(CMatrix4f &_m, float _angle);
CMatrix4f &MatrixRotationZ(CMatrix4f &_m, float _angle);

} // namespace drash

#endif // DRASH_MATRIX4_H
