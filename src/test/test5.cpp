// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "test5.h"

#include "../debugdrawsystem/camera.h"

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

    SetupCam();
    SetupProcessors();
    SetupMesh();

    return true;
}

void CTest5::Render()
{
    CApp::Render();

    GetRenderer().RenderMesh(mMesh);
}

void CTest5::SetupCam()
{
    auto cam = GetDebugDrawSystem().GetActiveCam();

    if (cam != nullptr)
    {
        cam->GetPos().Set(CVec3f(0, 0, 100));
    }
}

void CTest5::SetupMesh()
{
    mMesh = GetMeshManager().CreateMeshBox();
}

void CTest5::SetupProcessors()
{
    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));
}

} // namespace test

} // namespace drash
