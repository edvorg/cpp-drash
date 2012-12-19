#ifndef DRASH_PLANE_H
#define DRASH_PLANE_H

#include "vec3.h"

namespace drash
{

class CPlane
{
public:
    CPlane();

    void SetNormal(const CVec3f &_normal);
    inline const CVec3f &GetNormal() const;

    int CastRay(const CVec3f &, CVec3f &) const;

    CVec3f mPoint;

private:
    CVec3f mNormal;
};

inline const CVec3f &CPlane::GetNormal() const
{
    return mNormal;
}

} // namespace drash

#endif // DRASH_PLANE_H
