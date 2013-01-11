#include "test9.h"

#include "../greng/camera.h"

namespace drash
{

namespace test
{

bool CTest9::Init()
{
    if (CApp::Init() == false ||
        InitCamera() == false ||
        InitLights() == false ||
        InitRotationablePoint() == false)
    {
        return false;
    }

    return true;
}

void CTest9::Step(double _dt)
{
    CApp::Step(_dt);

    mPoint1.SetCursorPos(GetCursorPos());
    mPoint1.Step(_dt);
}

void CTest9::Render()
{
    CApp::Render();

    mPoint1.Render();
}

bool CTest9::InitCamera()
{
    greng::CCameraParams p;
    p.mPos.Set(10, 10, 10);
    p.mFov = M_PI / 6.0;
    mCamera = GetCameraManager().CreateCamera(p);
    mCamera->LookAt(CVec3f(0));

    if (mCamera == nullptr)
    {
        return false;
    }

    GetDebugRenderer().SetCamera(mCamera);

    return true;
}

bool CTest9::InitLights()
{
    mLight1.mPosition.Set(0, 10, 0);
    GetDebugRenderer().SetLight(&mLight1);

    return true;
}

bool CTest9::InitRotationablePoint()
{
    mPoint1.SetCamera(mCamera);
    mPoint1.SetRenderer(&GetRenderer());

    if (mPoint1.Init() == false)
    {
        return false;
    }

    mPoint1.SetPoint(CVec3f(0));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        mPoint1.RotateBegin();
    },
    [] ()
    {
    },
    [this] ()
    {
        mPoint1.RotateEnd();
    }));

    return true;
}

} // namespace test

} // namespace drash
