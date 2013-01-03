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
    if (CTest1::Init() == false)
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
    CTest1::Render();

    static float angle = 0;

    CApp::Render();

    if (mMesh1 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, angle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f model;
        MatrixMultiply(r, s, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        GetRenderer().RenderMesh(mMesh1, model_view);
    }

    if (mMesh1 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, -angle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(50, 0, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        GetRenderer().RenderMesh(mMesh2, model_view);
    }

    angle += 1.0 * GetCurrentTimeDelta();
}

void CTest5::SetupCam()
{
    auto cam = GetDebugDrawSystem().GetActiveCam();

    if (cam != nullptr)
    {
        cam->GetPos().Set(CVec3f(0, 0, 10));
    }
}

void CTest5::SetupMesh()
{
    mMesh1 = GetMeshManager().CreateMeshCube();
    mMesh2 = GetMeshManager().CreateMeshQuad();
}

void CTest5::SetupProcessors()
{
    GetEventSystem().SetMode("editor_mode");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        GetEventSystem().SetMode("test5");
    }));

    GetEventSystem().SetMode("test5");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        GetEventSystem().SetMode("editor_mode");
    }));
}

} // namespace test

} // namespace drash
