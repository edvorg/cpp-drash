#ifndef DRASH_PLANE_H
#define DRASH_PLANE_H

#include "vec3.h"

namespace drash
{

class CPlane
{
public:
    CPlane();

    void SetPoint(const CVec3f &_point);
    inline const CVec3f &GetPoint() const;
    void SetNormal(const CVec3f &_normal);
    inline const CVec3f &GetNormal() const;

    int CastRay(const CVec3f &, CVec3f &) const;

private:
    void ComputeD();

    CVec3f mPoint;
    CVec3f mNormal;
    float mD = 0;
};

inline const CVec3f &CPlane::GetPoint() const
{
    return mNormal;
}

inline const CVec3f &CPlane::GetNormal() const
{
    return mNormal;
}

} // namespace drash

#endif // DRASH_PLANE_H
