#include "plane.h"

namespace drash
{

CPlane::CPlane()
{
}

void CPlane::SetPoint(const CVec3f &_point)
{
    mPoint = _point;
    ComputeD();
}

void CPlane::SetNormal(const CVec3f &_normal)
{
    mNormal = _normal;
    mNormal.Normalize();
    ComputeD();
}

void CPlane::CastRay(const CRay &_ray, CVec3f &_result) const
{
    float tmp1 = _ray.GetDirection().mX * mNormal.mX+
                 _ray.GetDirection().mY * mNormal.mY +
                 _ray.GetDirection().mZ * mNormal.mZ;

    float tmp = (mNormal.mX * _ray.GetPoint().mX +
                 mNormal.mY * _ray.GetPoint().mY +
                 mNormal.mZ * _ray.GetPoint().mZ +
                 mD) / tmp1;

    _result.mX = _ray.GetPoint().mX - _ray.GetDirection().mX * tmp;
    _result.mY = _ray.GetPoint().mY - _ray.GetDirection().mY * tmp;
    _result.mZ = _ray.GetPoint().mZ - _ray.GetDirection().mZ * tmp;
}

void CPlane::ComputeD()
{
    // Ax + Bx + Cx + D = 0
    mD = - mNormal.mX * mPoint.mX - mNormal.mY * mPoint.mY - mNormal.mZ * mPoint.mZ;
}

} // namespace drash
