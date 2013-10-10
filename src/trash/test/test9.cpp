// DRASH_LICENSE_BEGIN
/*

  drash GPL Source Code
  Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

  This file is part of the drash GPL Source Code (drash Source Code).

  drash Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  drash Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
    mCamera = GetGrengSystems().GetCameraManager().CreateCamera(p);
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
    mPoint1.SetRenderer(&GetGrengSystems().GetRenderer());

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
