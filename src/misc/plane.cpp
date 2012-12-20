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

int CPlane::CastRay(const CVec3f &, CVec3f &) const
{
    return false;
}

void CPlane::ComputeD()
{
    // Ax + Bx + Cx + D = 0
    mD = - mNormal.mX * mPoint.mX - mNormal.mY * mPoint.mY - mNormal.mZ * mPoint.mZ;
}

} // namespace drash
