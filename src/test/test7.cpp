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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "test7.h"
#include "../greng/vertex.h"
#include "../scene/figure.h"
#include "../greng/camera.h"
#include "../scene/sceneobject.h"

namespace drash
{

namespace test
{

bool CTest7::Init()
{
    if (CTest3::Init() == false)
    {
        return false;
    }

    if (InitTextures() == false)
    {
        return false;
    }

    if (InitShaders() == false)
    {
        return false;
    }

    if (InitLights() == false)
    {
        return false;
    }

    mLight1.mPosition.Set(0, 30, 0);

    GetDebugRenderer().SetLight(&mLight1);

    return true;
}

void CTest7::Step(double _dt)
{
    CTest3::Step(_dt);

    mPointLight1PosAngle += _dt * 0.5;

    while (mPointLight1PosAngle >= M_PI * 2.0)
    {
        mPointLight1PosAngle -= M_PI * 2.0;
    }

    mLight1.mPosition.mX = 150 * sin(mPointLight1PosAngle);
}

bool CTest7::InitTextures()
{
    mDebugTexture = GetTextureManager().CreateTextureFromFile("assets/wall5.png");

    if (mDebugTexture == nullptr)
    {
        return false;
    }

    return true;
}

bool CTest7::InitShaders()
{
    greng::CVertexShader *v = GetVertexShaderManager().CreateShaderFromFile("shaders/shader2.120.vs");
    greng::CFragmentShader *f = GetFragmentShaderManager().CreateShaderFromFile("shaders/shader2.120.fs");

    mProgram = GetShaderProgramManager().CreateProgram(v, f);

    if (mProgram == nullptr)
    {
        return false;
    }

    return true;
}

bool CTest7::InitLights()
{
    mLight1.mPosition.Set(0, 50, 0);

    return true;
}

void CTest7::Render()
{
    unsigned int ic = GetScene().EnumObjects();

    for (unsigned int i = 0; i < ic; i++)
    {
        CSceneObject *o = GetScene().GetObjects()[i];
        unsigned int jc = o->EnumFigures();

        for (unsigned int j = 0; j < jc; j++)
        {
            CFigure *f = o->GetFigures()[j];
            unsigned int kc = f->EnumVertices();

            std::vector<greng::CVertex> mv;
            std::vector<unsigned int> mi;

            greng::CMesh *m = nullptr;

            if (kc >= 3)
            {
                CVec3f min(f->GetVertices()[0], f->GetZ() - 0.5f * f->GetDepth());
                CVec3f max(f->GetVertices()[0], f->GetZ() + 0.5f * f->GetDepth());

                for (unsigned int k = 1; k < kc; k++)
                {
                    min.mX = math::Min<float>(min.mX, f->GetVertices()[k].mX);
                    min.mY = math::Min<float>(min.mY, f->GetVertices()[k].mY);
                    max.mX = math::Max<float>(max.mX, f->GetVertices()[k].mX);
                    max.mY = math::Max<float>(max.mY, f->GetVertices()[k].mY);
                }

                max.Vec2() -= min.Vec2();

                mv.resize(kc * 2 + kc * 4);

                // front and back faces

                for (unsigned int k = 0; k < kc; k++)
                {
                    mv[k].mPos = CVec3f(f->GetVertices()[k], max.mZ);
                    mv[kc + k].mPos = CVec3f(f->GetVertices()[k], min.mZ);

                    mv[k].mUV = mv[k].mPos;
                    mv[kc + k].mUV = mv[kc + k].mPos;

                    mv[k].mUV -= min.Vec2();
                    mv[kc + k].mUV -= min.Vec2();

                    mv[k].mUV /= 10;
                    mv[kc + k].mUV /= 10;
                }

                for (unsigned int k = 2; k < kc; k++)
                {
                    mi.push_back(0);
                    mi.push_back(k - 1);
                    mi.push_back(k);
                }

                for (unsigned int k = 2; k < kc; k++)
                {
                    mi.push_back(kc);
                    mi.push_back(kc + k);
                    mi.push_back(kc + k - 1);
                }

                // sides

                for (unsigned int k = 1; k < kc; k++)
                {
                    mv[2 * kc + (k - 1) * 4 + 0].mPos = CVec3f(f->GetVertices()[k - 1], max.mZ);
                    mv[2 * kc + (k - 1) * 4 + 1].mPos = CVec3f(f->GetVertices()[k - 1], min.mZ);
                    mv[2 * kc + (k - 1) * 4 + 2].mPos = CVec3f(f->GetVertices()[k], min.mZ);
                    mv[2 * kc + (k - 1) * 4 + 3].mPos = CVec3f(f->GetVertices()[k], max.mZ);

                    CVec2f tmp = f->GetVertices()[k - 1];
                    tmp -= f->GetVertices()[k];

                    mv[2 * kc + (k - 1) * 4 + 0].mUV.Set(0, 0);
                    mv[2 * kc + (k - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) / 10.0f);
                    mv[2 * kc + (k - 1) * 4 + 2].mUV.Set(tmp.Length() / 10.0f, math::Abs(max.mZ - min.mZ) / 10.0f);
                    mv[2 * kc + (k - 1) * 4 + 3].mUV.Set(tmp.Length() / 10.0f, 0);

                    mi.push_back(2 * kc + (k - 1) * 4 + 0);
                    mi.push_back(2 * kc + (k - 1) * 4 + 1);
                    mi.push_back(2 * kc + (k - 1) * 4 + 2);
                    mi.push_back(2 * kc + (k - 1) * 4 + 2);
                    mi.push_back(2 * kc + (k - 1) * 4 + 3);
                    mi.push_back(2 * kc + (k - 1) * 4 + 0);
                }
                mv[2 * kc + (kc - 1) * 4 + 0].mPos = CVec3f(f->GetVertices()[kc - 1], max.mZ);
                mv[2 * kc + (kc - 1) * 4 + 1].mPos = CVec3f(f->GetVertices()[kc - 1], min.mZ);
                mv[2 * kc + (kc - 1) * 4 + 2].mPos = CVec3f(f->GetVertices()[0], min.mZ);
                mv[2 * kc + (kc - 1) * 4 + 3].mPos = CVec3f(f->GetVertices()[0], max.mZ);

                CVec2f tmp = f->GetVertices()[kc - 1];
                tmp -= f->GetVertices()[0];

                mv[2 * kc + (kc - 1) * 4 + 0].mUV.Set(0, 0);
                mv[2 * kc + (kc - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) / 10.0f);
                mv[2 * kc + (kc - 1) * 4 + 2].mUV.Set(tmp.Length() / 10.0f, math::Abs(max.mZ - min.mZ) / 10.0f);
                mv[2 * kc + (kc - 1) * 4 + 3].mUV.Set(tmp.Length() / 10.0f, 0);

                mi.push_back(2 * kc + (kc - 1) * 4 + 0);
                mi.push_back(2 * kc + (kc - 1) * 4 + 1);
                mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                mi.push_back(2 * kc + (kc - 1) * 4 + 3);
                mi.push_back(2 * kc + (kc - 1) * 4 + 0);

                m = GetMeshManager().CreateMeshFromVertices(&mv[0], mv.size(), &mi[0], mi.size());
            }

            if (m != nullptr)
            {
                GetMeshManager().ComputeNormals(m);

                CMatrix4f rot;
                MatrixRotationZ(rot, o->GetAngle());

                CMatrix4f trans;
                MatrixTranslation(trans, o->GetPos());

                CMatrix4f model;
                MatrixMultiply(trans, rot, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

                GetRenderer().RenderMesh(m,
                                         0,
                                         &mDebugTexture,
                                         1,
                                         mProgram,
                                         &model,
                                         nullptr,
                                         &model_view,
                                         &GetCamera()->GetProjectionMatrix(),
                                         &mLight1);

                GetMeshManager().DestroyMesh(m);
            }
        }
    }
}

} // namespace test

} // namespace drash
