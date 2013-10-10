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

#include "test5.h"

#include "../greng/camera.h"
#include "../players/player.h"

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

    SetupProcessors();
    SetupMeshes();
    SetupTextures();
    SetupShaders();
    SetupLights();

    mLight1.mPosition.Set(0, 50, 0);

    GetDebugRenderer().SetLight(&mLight1);

    return true;
}

void CTest5::Step(double _dt)
{
    CTest3::Step(_dt);

    mAngle += 0.5 * _dt;

    while (mAngle > M_PI * 2.0)
    {
        mAngle -= M_PI * 2.0;
    }

    mLight1.mPosition.mX = 200 * sin(mAngle);
    mLight1.mPosition.mY = 50;
    mLight1.mPosition.mZ = 200 * cos(mAngle);
}

void CTest5::Render()
{
    CTest3::Render();

    if (mMesh1 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, mAngle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(-100, 30, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

        GetGrengSystems().GetRenderer().RenderMesh(mMesh1,
                                                     0,
                                                     &mTex6,
                                                     1,
                                                     mShaderProgram2,
                                                     &model,
                                                     nullptr,
                                                     &model_view,
                                                     &GetCamera()->GetProjectionMatrix(),
                                                     &mLight1);
    }

    if (mMesh2 != nullptr)
    {
        CMatrix4f r;
        MatrixRotationZ(r, -mAngle);

        CMatrix4f s;
        MatrixScale(s, CVec3f(10));

        CMatrix4f rot;
        MatrixMultiply(r, s, rot);

        CMatrix4f transl;
        MatrixTranslation(transl, CVec3f(100, 30, 0));

        CMatrix4f model;
        MatrixMultiply(transl, rot, model);

        CMatrix4f model_view;
        MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

        GetGrengSystems().GetRenderer().RenderMesh(mMesh2,
                                 0,
                                 &mTex2,
                                 1,
                                 mShaderProgram2,
                                 &model,
                                 nullptr,
                                 &model_view,
                                 &GetCamera()->GetProjectionMatrix(),
                                 &mLight1);
    }

    if (mMesh3 != nullptr)
    {
        CMatrix4f rangle;
        MatrixRotationY(rangle, 0);

        CMatrix4f model;
        MatrixMultiply(rangle, mMesh3ConstMatrix, model);

        CMatrix4f model_view;
        MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

        greng::CTexture *texts[6] =
        {
            mTex4, mTex4normal,
            mTex3, mTex3normal,
            mTex5, mTex5normal
        };

        for (unsigned int i = 0; i < 3; i++)
        {
            GetGrengSystems().GetRenderer().RenderMesh(mMesh3,
                                     i,
                                     &texts[i * 2],
                                     2,
                                     mShaderProgram4,
                                     &model,
                                     nullptr,
                                     &model_view,
                                     &GetCamera()->GetProjectionMatrix(),
                                     &mLight1,
                                     nullptr,
                                     &GetCamera()->GetPos().Get());
        }
    }

    if (mMesh4 != nullptr)
    {
        CMatrix4f rangle;
        MatrixRotationY(rangle, 0);

        CMatrix4f model_1;
        MatrixMultiply(rangle, mMesh3ConstMatrix, model_1);

        CMatrix4f trans;
        MatrixTranslation(trans, CVec3f(-150, 0, 0));

        CMatrix4f model;
        MatrixMultiply(trans, model_1, model);

        CMatrix4f model_view;
        MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

        greng::CTexture *texts[6] =
        {
            mTex7, mTex7normal,
            mTex7, mTex7normal,
            mTex7, mTex7normal,
        };

        for (unsigned int i = 0; i < 3; i++)
        {
            GetGrengSystems().GetRenderer().RenderMesh(mMesh4,
                                     i,
                                     &texts[i * 2],
                                     2,
                                     mShaderProgram4,
                                     &model,
                                     nullptr,
                                     &model_view,
                                     &GetCamera()->GetProjectionMatrix(),
                                     &mLight1,
                                     nullptr,
                                     &GetCamera()->GetPos().Get());
        }
    }

    GetGrengSystems().GetRenderer().DrawPoint(GetCamera(), mLight1.mPosition, 10, CColor4f(1, 1, 1, 1), false);
}

void CTest5::SetupMeshes()
{
    mMesh1 = GetGrengSystems().GetMeshManager().CreateMeshCube();
    mMesh2 = GetGrengSystems().GetMeshManager().CreateMeshQuad();
    mMesh3 = GetGrengSystems().GetMeshManager().CreateMeshFromObjFile("assets/mt.obj");
    mMesh4 = GetGrengSystems().GetMeshManager().CreateMeshFromObjFile("assets/RB-BumbleBee.obj");

    GetGrengSystems().GetMeshManager().ComputeNormals(mMesh3);
    GetGrengSystems().GetMeshManager().ComputeTangentSpace(mMesh3);
    GetGrengSystems().GetMeshManager().ComputeNormals(mMesh4);

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
    mTex1 = GetGrengSystems().GetTextureManager().CreateTextureDummy();
    mTex2 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/tux.png");
    mTex3 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/RB_MegatronBodyT2_MATINST.png");
    mTex3normal = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/mt_body_normal.png");
    mTex4 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/RB_MegatronHead_MATINST.png");
    mTex4normal = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/mt_head_normal.png");
    mTex5 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/RB_MegatronArmFeet_MATINST.png");
    mTex5normal = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/mt_arm_normal.png");
    mTex6 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/wall5.png");
    mTex7 = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/RB_Bumblebee_TEXTSET_Color_NormX.png");
    mTex7normal = GetGrengSystems().GetTextureManager().CreateTextureFromFile("assets/bb_normal.png");
}

void CTest5::SetupShaders()
{
    mVertexShader1 = GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile("shaders/shader1.120.vs");
    mFragmentShader1 = GetGrengSystems().GetFragmentShaderManager().CreateShaderFromFile("shaders/shader1.120.fs");
    mShaderProgram1 = GetGrengSystems().GetShaderProgramManager().CreateProgram(mVertexShader1, mFragmentShader1);
    mVertexShader2 = GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile("shaders/shader2.120.vs");
    mFragmentShader2 = GetGrengSystems().GetFragmentShaderManager().CreateShaderFromFile("shaders/shader2.120.fs");
    mShaderProgram2 = GetGrengSystems().GetShaderProgramManager().CreateProgram(mVertexShader2, mFragmentShader2);
    mVertexShader3 = GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile("shaders/shader3.120.vs");
    mFragmentShader3 = GetGrengSystems().GetFragmentShaderManager().CreateShaderFromFile("shaders/shader3.120.fs");
    mShaderProgram3 = GetGrengSystems().GetShaderProgramManager().CreateProgram(mVertexShader3, mFragmentShader3);
    mVertexShader4 = GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile("shaders/shader4.120.vs");
    mFragmentShader4 = GetGrengSystems().GetFragmentShaderManager().CreateShaderFromFile("shaders/shader4.120.fs");
    mShaderProgram4 = GetGrengSystems().GetShaderProgramManager().CreateProgram(mVertexShader4, mFragmentShader4);
}

void CTest5::SetupProcessors()
{
}

void CTest5::SetupLights()
{
    mLight1.mPosition.Set(-50, 100, 0);
}

} // namespace test

} // namespace drash
