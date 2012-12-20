#ifndef DRASH_RAY_H
#define DRASH_RAY_H

#include "vec3.h"

namespace drash
{

class CRay
{
public:
    CRay();

    void SetPoint(const CVec3f &_point);
    inline const CVec3f &GetPoint() const;
    void SetDirection(const CVec3f &_direction);
    inline const CVec3f &GetDirection() const;

private:
    CVec3f mPoint;
    CVec3f mDirection;
};

inline const CVec3f &CRay::GetPoint() const
{
    return mPoint;
}

inline const CVec3f &CRay::GetDirection() const
{
    return mDirection;
}

} // namespace drash

#endif // DRASH_RAY_H
