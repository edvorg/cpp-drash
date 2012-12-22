#include "matrix4.h"

namespace drash
{

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

} // namespace drash
