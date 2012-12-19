#include "plane.h"

namespace drash
{

CPlane::CPlane()
{
}

void CPlane::SetNormal(const CVec3f &_normal)
{
    mNormal = _normal;
    mNormal.Normalize();
}

int CPlane::CastRay(const CVec3f &, CVec3f &) const
{
    return false;
}

} // namespace drash
