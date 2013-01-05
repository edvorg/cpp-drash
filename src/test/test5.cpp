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
    if (CTest3::Init() == false)
    {
        return false;
    }

    SetupCam();
    SetupProcessors();
    SetupMeshes();
    SetupTextures();
    SetupShaders();

    return true;
}

void CTest5::Render()
{
    CTest3::Render();

    static float angle = 0;

    if (mMesh1 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, angle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(-100, 30, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        GetRenderer().RenderMesh(mMesh1,
                                 0,
                                 mTex6,
                                 mShaderProgram1,
                                 model_view,
                                 GetDebugDrawSystem().GetActiveCam()->GetProjectionMatrix());
    }

    if (mMesh2 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, -angle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(100, 30, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        GetRenderer().RenderMesh(mMesh2,
                                 0,
                                 mTex2,
                                 mShaderProgram1,
                                 model_view,
                                 GetDebugDrawSystem().GetActiveCam()->GetProjectionMatrix());
    }

    if (mMesh3 != nullptr)
    {
        CMatrix4f rangle;
        MatrixRotationY(rangle, angle);

        CMatrix4f model;
        MatrixMultiply(rangle, mMesh3ConstMatrix, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        greng::CTexture *texts[3] =
        {
            mTex4,
            mTex3,
            mTex5
        };

        for (unsigned int i = 0; i < 3; i++)
        {
            GetRenderer().RenderMesh(mMesh3,
                                     i,
                                     texts[i],
                                     mShaderProgram1,
                                     model_view,
                                     GetDebugDrawSystem().GetActiveCam()->GetProjectionMatrix());
        }
    }
    if (mMesh4 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationY(r, -angle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(1));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(200, 30, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

        for (unsigned int i = 0; i < 6; i++)
        {
            GetRenderer().RenderMesh(mMesh4,
                                     i,
                                     mTex1,
                                     mShaderProgram1,
                                     model_view,
                                     GetDebugDrawSystem().GetActiveCam()->GetProjectionMatrix());
        }
    }

    angle += 1.0 * GetCurrentTimeDelta();
}

void CTest5::SetupCam()
{
    auto cam = GetDebugDrawSystem().GetActiveCam();

    if (cam != nullptr)
    {
    }
}

void CTest5::SetupMeshes()
{
    mMesh1 = GetMeshManager().CreateMeshCube();
    mMesh2 = GetMeshManager().CreateMeshQuad();
    mMesh3 = GetMeshManager().CreateMeshFromObjFile("mt.obj");
    mMesh4 = GetMeshManager().CreateMeshFromObjFile("player.obj");

    CMatrix4f s;
    MatrixScale(s, CVec3f(0.1));

    CMatrix4f rx;
    MatrixRotationX(rx, - M_PI / 2.0);

    CMatrix4f ry;
    MatrixRotationY(ry, - M_PI / 2.0);

    CMatrix4f rxy;
    MatrixMultiply(ry, rx, rxy);

    MatrixMultiply(rxy, s, mMesh3ConstMatrix);
}

void CTest5::SetupTextures()
{
    mTex1 = GetTextureManager().CreateTextureDummy();
    mTex2 = GetTextureManager().CreateTextureFromFile("tux.png");
    mTex3 = GetTextureManager().CreateTextureFromFile("RB_MegatronBodyT2_MATINST.png");
    mTex4 = GetTextureManager().CreateTextureFromFile("RB_MegatronHead_MATINST.png");
    mTex5 = GetTextureManager().CreateTextureFromFile("RB_MegatronArmFeet_MATINST.png");
    mTex6 = GetTextureManager().CreateTextureFromFile("wall5.png");
}

void CTest5::SetupShaders()
{
    mVertexShader1 = GetVertexShaderManager().CreateShaderFromFile("shader1.120.vs");
    mFragmentShader1 = GetFragmentShaderManager().CreateShaderFromFile("shader1.120.fs");
    mShaderProgram1 = GetShaderProgramManager().CreateProgram(mVertexShader1, mFragmentShader1);
}

void CTest5::SetupProcessors()
{
}

} // namespace test

} // namespace drash
