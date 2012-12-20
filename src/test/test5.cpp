#include "test5.h"

#include "../debugdrawsystem/camera.h"

#include "../misc/plane.h"

namespace drash
{

namespace test
{

bool CTest5::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    CPlane p;
    p.SetNormal(CVec3f(0, 0, 1));
    p.SetPoint(CVec3f(0));

    CVec3f cast_result;
    CRay r;

    r.SetDirection(CVec3f(1, 1, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(1, 0, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(-1, 0, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(0, -1, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    this->Quit();

    return true;
}

} // namespace test
} // namespace drash
