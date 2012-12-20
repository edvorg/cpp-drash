#include "ray.h"

namespace drash
{

CRay::CRay()
{
}

void CRay::SetPoint(const CVec3f &_point)
{
    mPoint = _point;
}

void CRay::SetDirection(const CVec3f &_direction)
{
    mDirection = _direction;
    mDirection.Normalize();
}

} // namespace drash
