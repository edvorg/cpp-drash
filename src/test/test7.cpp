// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

namespace drash {

    namespace test {

        CTest7::CTest7() : CTest3() {
            InitTextures();
            InitShaders();
            InitLights();

            light1.position.Set(0, 30, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void CTest7::Step(double _dt) {
            CTest3::Step(_dt);

            pointLight1PosAngle += _dt * 0.5;

            while (pointLight1PosAngle >= M_PI * 2.0) {
                pointLight1PosAngle -= M_PI * 2.0;
            }

            light1.position.x = 150 * sin(pointLight1PosAngle);
        }

        bool CTest7::InitTextures() {
            debugTexture =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/floor/diffuse.png");

            if (debugTexture == nullptr) {
                return false;
            }

            return true;
        }

        bool CTest7::InitShaders() {
            greng::CVertexShader* v =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader2.120.vs");
            greng::CFragmentShader* f =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader2.120.fs");

            program =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(v, f);

            if (program == nullptr) {
                return false;
            }

            return true;
        }

        bool CTest7::InitLights() {
            light1.position.Set(0, 50, 0);

            return true;
        }

        void CTest7::Render() {
            unsigned int ic = GetScene().EnumObjects();

            for (unsigned int i = 0; i < ic; i++) {
                CSceneObject* o = GetScene().GetObjects()[i];
                unsigned int jc = o->EnumFigures();

                for (unsigned int j = 0; j < jc; j++) {
                    CFigure* f = o->GetFigures()[j];
                    unsigned int kc = f->EnumVertices();

                    std::vector<greng::CVertex> mv;
                    std::vector<unsigned int> mi;

                    greng::CMesh* m = nullptr;

                    if (kc >= 3) {
                        CVec3f min(f->GetVertices()[0],
                                   f->GetZ() - 0.5f * f->GetDepth());
                        CVec3f max(f->GetVertices()[0],
                                   f->GetZ() + 0.5f * f->GetDepth());

                        for (unsigned int k = 1; k < kc; k++) {
                            min.x =
                                math::Min<float>(min.x, f->GetVertices()[k].x);
                            min.y =
                                math::Min<float>(min.y, f->GetVertices()[k].y);
                            max.x =
                                math::Max<float>(max.x, f->GetVertices()[k].x);
                            max.y =
                                math::Max<float>(max.y, f->GetVertices()[k].y);
                        }

                        max.Vec2() -= min.Vec2();

                        mv.resize(kc * 2 + kc * 4);

                        // front and back faces

                        for (unsigned int k = 0; k < kc; k++) {
                            mv[k].pos = CVec3f(f->GetVertices()[k], max.z);
                            mv[kc + k].pos = CVec3f(f->GetVertices()[k], min.z);

                            mv[k].uV = mv[k].pos;
                            mv[kc + k].uV = mv[kc + k].pos;

                            mv[k].uV -= min.Vec2();
                            mv[kc + k].uV -= min.Vec2();

                            mv[k].uV /= 10;
                            mv[kc + k].uV /= 10;
                        }

                        for (unsigned int k = 2; k < kc; k++) {
                            mi.push_back(0);
                            mi.push_back(k - 1);
                            mi.push_back(k);
                        }

                        for (unsigned int k = 2; k < kc; k++) {
                            mi.push_back(kc);
                            mi.push_back(kc + k);
                            mi.push_back(kc + k - 1);
                        }

                        // sides

                        for (unsigned int k = 1; k < kc; k++) {
                            mv[2 * kc + (k - 1) * 4 + 0].pos =
                                CVec3f(f->GetVertices()[k - 1], max.z);
                            mv[2 * kc + (k - 1) * 4 + 1].pos =
                                CVec3f(f->GetVertices()[k - 1], min.z);
                            mv[2 * kc + (k - 1) * 4 + 2].pos =
                                CVec3f(f->GetVertices()[k], min.z);
                            mv[2 * kc + (k - 1) * 4 + 3].pos =
                                CVec3f(f->GetVertices()[k], max.z);

                            CVec2f tmp = f->GetVertices()[k - 1];
                            tmp -= f->GetVertices()[k];

                            mv[2 * kc + (k - 1) * 4 + 0].uV.Set(0, 0);
                            mv[2 * kc + (k - 1) * 4 + 1]
                                .uV.Set(0, math::Abs(max.z - min.z) / 10.0f);
                            mv[2 * kc + (k - 1) * 4 + 2]
                                .uV.Set(tmp.Length() / 10.0f,
                                        math::Abs(max.z - min.z) / 10.0f);
                            mv[2 * kc + (k - 1) * 4 + 3]
                                .uV.Set(tmp.Length() / 10.0f, 0);

                            mi.push_back(2 * kc + (k - 1) * 4 + 0);
                            mi.push_back(2 * kc + (k - 1) * 4 + 1);
                            mi.push_back(2 * kc + (k - 1) * 4 + 2);
                            mi.push_back(2 * kc + (k - 1) * 4 + 2);
                            mi.push_back(2 * kc + (k - 1) * 4 + 3);
                            mi.push_back(2 * kc + (k - 1) * 4 + 0);
                        }
                        mv[2 * kc + (kc - 1) * 4 + 0].pos =
                            CVec3f(f->GetVertices()[kc - 1], max.z);
                        mv[2 * kc + (kc - 1) * 4 + 1].pos =
                            CVec3f(f->GetVertices()[kc - 1], min.z);
                        mv[2 * kc + (kc - 1) * 4 + 2].pos =
                            CVec3f(f->GetVertices()[0], min.z);
                        mv[2 * kc + (kc - 1) * 4 + 3].pos =
                            CVec3f(f->GetVertices()[0], max.z);

                        CVec2f tmp = f->GetVertices()[kc - 1];
                        tmp -= f->GetVertices()[0];

                        mv[2 * kc + (kc - 1) * 4 + 0].uV.Set(0, 0);
                        mv[2 * kc + (kc - 1) * 4 + 1]
                            .uV.Set(0, math::Abs(max.z - min.z) / 10.0f);
                        mv[2 * kc + (kc - 1) * 4 + 2]
                            .uV.Set(tmp.Length() / 10.0f,
                                    math::Abs(max.z - min.z) / 10.0f);
                        mv[2 * kc + (kc - 1) * 4 + 3]
                            .uV.Set(tmp.Length() / 10.0f, 0);

                        mi.push_back(2 * kc + (kc - 1) * 4 + 0);
                        mi.push_back(2 * kc + (kc - 1) * 4 + 1);
                        mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                        mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                        mi.push_back(2 * kc + (kc - 1) * 4 + 3);
                        mi.push_back(2 * kc + (kc - 1) * 4 + 0);

                        m = GetGrengSystems()
                                .GetMeshManager()
                                .CreateMeshFromVertices(&mv[0], mv.size(),
                                                        &mi[0], mi.size());
                    }

                    if (m != nullptr) {
                        GetGrengSystems().GetMeshManager().ComputeNormals(m);

                        CMatrix4f rot;
                        MatrixRotationZ(rot, o->GetAngle());

                        CMatrix4f trans;
                        MatrixTranslation(trans, o->GetPos());

                        CMatrix4f model;
                        MatrixMultiply(trans, rot, model);

                        CMatrix4f model_view;
                        MatrixMultiply(GetCamera().GetViewMatrix(), model,
                                       model_view);

                        GetGrengSystems().GetRenderer().RenderMesh(
                            m, 0, &debugTexture, 1, program, &model, nullptr,
                            &model_view, &GetCamera().GetProjectionMatrix(),
                            &light1);

                        GetGrengSystems().GetMeshManager().DestroyMesh(m);
                    }
                }
            }
        }

    } // namespace test

} // namespace drash
