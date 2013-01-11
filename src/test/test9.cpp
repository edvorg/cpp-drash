#include "test9.h"

namespace drash
{

namespace test
{

bool CTest9::Init()
{
    if (CApp::Init() == false ||
        InitCamera() == false ||
        InitLights() == false)
    {
        return false;
    }

    return true;
}

bool CTest9::InitCamera()
{
    greng::CCameraParams p;
    p.mPos.Set(0, 0, 10);
    p.mFov = M_PI / 6.0;
    auto c = GetCameraManager().CreateCamera(p);
    GetDebugRenderer().SetCamera(c);

    return true;
}

bool CTest9::InitLights()
{
    mLight1.mPosition.Set(0, 10, 0);
    GetDebugRenderer().SetLight(&mLight1);

    return true;
}

} // namespace test

} // namespace drash
