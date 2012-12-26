#ifndef DRASH_PLANE_H
#define DRASH_PLANE_H

#include "vec3.h"

namespace drash
{

class CRay;

class CPlane
{
public:
    CPlane();
    CPlane(const CPlane &_plane);
    CPlane(const CVec3f &_point, const CVec3f &_normal);

    void SetPoint(const CVec3f &_point);
    inline const CVec3f &GetPoint() const;
    void SetNormal(const CVec3f &_normal);
    inline const CVec3f &GetNormal() const;

    void CastRay(const CRay &_ray, CVec3f &_result) const;

private:
    void ComputeD();

    CVec3f mPoint;
    CVec3f mNormal;
    float mD = 0;
};

extern const CPlane PlaneXY;
extern const CPlane PlaneYZ;
extern const CPlane PlaneXZ;

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
