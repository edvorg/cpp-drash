#include "matrix4.h"

#include <cstring>
#include <cmath>

namespace drash
{

CMatrix4f::CMatrix4f(const CMatrix4f &_m)
{
    memcpy(mData, _m.mData, sizeof(float) * mElemsCount);
}

CMatrix4f &CMatrix4f::Zero()
{
    memset(mData, 0, sizeof(float) * mElemsCount);
    return *this;
}

CMatrix4f &CMatrix4f::Identity()
{
    Zero();
    mData[m00] = mData[m11] = mData[m22] = mData[m33] = 1;
    return *this;
}

CMatrix4f &CMatrix4f::Transpose()
{
    float tmp = mData[m01];
    mData[m01] = mData[m10];
    mData[m10] = tmp;


    tmp = mData[m02];
    mData[m02] = mData[m20];
    mData[m20] = tmp;


    tmp = mData[m03];
    mData[m03] = mData[m30];
    mData[m30] = tmp;


    tmp = mData[m12];
    mData[m12] = mData[m21];
    mData[m21] = tmp;


    tmp = mData[m13];
    mData[m13] = mData[m31];
    mData[m31] = tmp;


    tmp = mData[m23];
    mData[m23] = mData[m32];
    mData[m32] = tmp;

    return *this;
}

CMatrix4f &MatrixMultiply(const CMatrix4f &_m1, const CMatrix4f &_m2, CMatrix4f &_result)
{
    _result.mData[_result.m00] = _m1.mData[_m1.m00] * _m2.mData[_m2.m00] +
                                    _m1.mData[_m1.m01] * _m2.mData[_m2.m10] +
                                    _m1.mData[_m1.m02] * _m2.mData[_m2.m20] +
                                    _m1.mData[_m1.m03] * _m2.mData[_m2.m30];

    _result.mData[_result.m01] = _m1.mData[_m1.m00] * _m2.mData[_m2.m01] +
                                    _m1.mData[_m1.m01] * _m2.mData[_m2.m11] +
                                    _m1.mData[_m1.m02] * _m2.mData[_m2.m21] +
                                    _m1.mData[_m1.m03] * _m2.mData[_m2.m31];

    _result.mData[_result.m02] = _m1.mData[_m1.m00] * _m2.mData[_m2.m02] +
                                    _m1.mData[_m1.m01] * _m2.mData[_m2.m12] +
                                    _m1.mData[_m1.m02] * _m2.mData[_m2.m22] +
                                    _m1.mData[_m1.m03] * _m2.mData[_m2.m32];

    _result.mData[_result.m03] = _m1.mData[_m1.m00] * _m2.mData[_m2.m03] +
                                    _m1.mData[_m1.m01] * _m2.mData[_m2.m13] +
                                    _m1.mData[_m1.m02] * _m2.mData[_m2.m23] +
                                    _m1.mData[_m1.m03] * _m2.mData[_m2.m33];



    _result.mData[_result.m10] = _m1.mData[_m1.m10] * _m2.mData[_m2.m00] +
                                    _m1.mData[_m1.m11] * _m2.mData[_m2.m10] +
                                    _m1.mData[_m1.m12] * _m2.mData[_m2.m20] +
                                    _m1.mData[_m1.m13] * _m2.mData[_m2.m30];

    _result.mData[_result.m11] = _m1.mData[_m1.m10] * _m2.mData[_m2.m01] +
                                    _m1.mData[_m1.m11] * _m2.mData[_m2.m11] +
                                    _m1.mData[_m1.m12] * _m2.mData[_m2.m21] +
                                    _m1.mData[_m1.m13] * _m2.mData[_m2.m31];

    _result.mData[_result.m12] = _m1.mData[_m1.m10] * _m2.mData[_m2.m02] +
                                    _m1.mData[_m1.m11] * _m2.mData[_m2.m12] +
                                    _m1.mData[_m1.m12] * _m2.mData[_m2.m22] +
                                    _m1.mData[_m1.m13] * _m2.mData[_m2.m32];

    _result.mData[_result.m13] = _m1.mData[_m1.m10] * _m2.mData[_m2.m03] +
                                    _m1.mData[_m1.m11] * _m2.mData[_m2.m13] +
                                    _m1.mData[_m1.m12] * _m2.mData[_m2.m23] +
                                    _m1.mData[_m1.m13] * _m2.mData[_m2.m33];



    _result.mData[_result.m20] = _m1.mData[_m1.m20] * _m2.mData[_m2.m00] +
                                    _m1.mData[_m1.m21] * _m2.mData[_m2.m10] +
                                    _m1.mData[_m1.m22] * _m2.mData[_m2.m20] +
                                    _m1.mData[_m1.m23] * _m2.mData[_m2.m30];

    _result.mData[_result.m21] = _m1.mData[_m1.m20] * _m2.mData[_m2.m01] +
                                    _m1.mData[_m1.m21] * _m2.mData[_m2.m11] +
                                    _m1.mData[_m1.m22] * _m2.mData[_m2.m21] +
                                    _m1.mData[_m1.m23] * _m2.mData[_m2.m31];

    _result.mData[_result.m22] = _m1.mData[_m1.m20] * _m2.mData[_m2.m02] +
                                    _m1.mData[_m1.m21] * _m2.mData[_m2.m12] +
                                    _m1.mData[_m1.m22] * _m2.mData[_m2.m22] +
                                    _m1.mData[_m1.m23] * _m2.mData[_m2.m32];

    _result.mData[_result.m23] = _m1.mData[_m1.m20] * _m2.mData[_m2.m03] +
                                    _m1.mData[_m1.m21] * _m2.mData[_m2.m13] +
                                    _m1.mData[_m1.m22] * _m2.mData[_m2.m23] +
                                    _m1.mData[_m1.m23] * _m2.mData[_m2.m33];



    _result.mData[_result.m30] = _m1.mData[_m1.m30] * _m2.mData[_m2.m00] +
                                    _m1.mData[_m1.m31] * _m2.mData[_m2.m10] +
                                    _m1.mData[_m1.m32] * _m2.mData[_m2.m20] +
                                    _m1.mData[_m1.m33] * _m2.mData[_m2.m30];

    _result.mData[_result.m31] = _m1.mData[_m1.m30] * _m2.mData[_m2.m01] +
                                    _m1.mData[_m1.m31] * _m2.mData[_m2.m11] +
                                    _m1.mData[_m1.m32] * _m2.mData[_m2.m21] +
                                    _m1.mData[_m1.m33] * _m2.mData[_m2.m31];

    _result.mData[_result.m32] = _m1.mData[_m1.m30] * _m2.mData[_m2.m02] +
                                    _m1.mData[_m1.m31] * _m2.mData[_m2.m12] +
                                    _m1.mData[_m1.m32] * _m2.mData[_m2.m22] +
                                    _m1.mData[_m1.m33] * _m2.mData[_m2.m32];

    _result.mData[_result.m33] = _m1.mData[_m1.m30] * _m2.mData[_m2.m03] +
                                    _m1.mData[_m1.m31] * _m2.mData[_m2.m13] +
                                    _m1.mData[_m1.m32] * _m2.mData[_m2.m23] +
                                    _m1.mData[_m1.m33] * _m2.mData[_m2.m33];



    return _result;
}

CVec4f &MatrixMultiply(const CVec4f &_v, const CMatrix4f &_m, CVec4f &_result)
{
    _result.mX = _v.mX * _m.mData[_m.m00] +
                    _v.mY * _m.mData[_m.m10] +
                    _v.mZ * _m.mData[_m.m20] +
                    _v.mW * _m.mData[_m.m30];


    _result.mY = _v.mX * _m.mData[_m.m01] +
                    _v.mY * _m.mData[_m.m11] +
                    _v.mZ * _m.mData[_m.m21] +
                    _v.mW * _m.mData[_m.m31];


    _result.mZ = _v.mX * _m.mData[_m.m02] +
                    _v.mY * _m.mData[_m.m12] +
                    _v.mZ * _m.mData[_m.m22] +
                    _v.mW * _m.mData[_m.m32];


    _result.mW = _v.mX * _m.mData[_m.m03] +
                    _v.mY * _m.mData[_m.m13] +
                    _v.mZ * _m.mData[_m.m23] +
                    _v.mW * _m.mData[_m.m33];


    return _result;
}

CMatrix4f &MatrixRotationX(CMatrix4f &_m, float _angle)
{
    float c = cos(_angle);
    float s = sin(_angle);

    _m.Identity();
    _m.mData[_m.m11] = _m.mData[_m.m22] = c;
    _m.mData[_m.m12] = s;
    _m.mData[_m.m21] = -s;

    return _m;
}

CMatrix4f &MatrixRotationY(CMatrix4f &_m, float _angle)
{
    float c = cos(_angle);
    float s = sin(_angle);

    _m.Identity();
    _m.mData[_m.m00] = _m.mData[_m.m22] = c;
    _m.mData[_m.m02] = -s;
    _m.mData[_m.m20] = s;

    return _m;
}

CMatrix4f &MatrixRotationZ(CMatrix4f &_m, float _angle)
{
    float c = cos(_angle);
    float s = sin(_angle);

    _m.Identity();
    _m.mData[_m.m00] = _m.mData[_m.m11] = c;
    _m.mData[_m.m01] = s;
    _m.mData[_m.m10] = -s;

    return _m;
}

CMatrix4f &MatrixTranslation(CMatrix4f &_m, const CVec3f &_translation)
{
    _m.Identity();

    _m.mData[_m.m30] = _translation.mX;
    _m.mData[_m.m31] = _translation.mY;
    _m.mData[_m.m32] = _translation.mZ;

    return _m;
}

} // namespace drash
