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

#include "test5.h"

#include "../greng/camera.h"
#include "../players/player.h"

namespace drash {

    namespace test {

        CTest5::CTest5(greng::CGrengSystemsSet& greng) : CTest3(greng) {
            SetupProcessors();
            SetupMeshes();
            SetupTextures();
            SetupShaders();
            SetupLights();

            light1.position.Set(0, 50, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void CTest5::Step(double _dt) {
            CTest3::Step(_dt);

            angle += 0.5 * _dt;

            while (angle > M_PI * 2.0) {
                angle -= M_PI * 2.0;
            }

            light1.position.x = 200 * sin(angle);
            light1.position.y = 50;
            light1.position.z = 200 * cos(angle);
        }

        void CTest5::Render() {
            CTest3::Render();

            if (mesh1 != nullptr) {
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
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                GetGrengSystems().GetRenderer().RenderMesh(
                    mesh1, 0, &tex6, 1, shaderProgram2, &model, nullptr,
                    &model_view, &GetCamera().GetProjectionMatrix(), &light1);
            }

            if (mesh2 != nullptr) {
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
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                GetGrengSystems().GetRenderer().RenderMesh(
                    mesh2, 0, &tex2, 1, shaderProgram2, &model, nullptr,
                    &model_view, &GetCamera().GetProjectionMatrix(), &light1);
            }

            if (mesh3 != nullptr) {
                CMatrix4f rangle;
                MatrixRotationY(rangle, 0);

                CMatrix4f model;
                MatrixMultiply(rangle, mesh3ConstMatrix, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                greng::CTexture* texts[6] = {
                    tex4, tex4normal, tex3, tex3normal, tex5, tex5normal
                };

                for (unsigned int i = 0; i < 3; i++) {
                    GetGrengSystems().GetRenderer().RenderMesh(
                        mesh3, i, &texts[i * 2], 2, shaderProgram4, &model,
                        nullptr, &model_view,
                        &GetCamera().GetProjectionMatrix(), &light1, nullptr,
                        &GetCamera().GetPos().Get());
                }
            }

            if (mesh4 != nullptr) {
                CMatrix4f rangle;
                MatrixRotationY(rangle, 0);

                CMatrix4f model_1;
                MatrixMultiply(rangle, mesh3ConstMatrix, model_1);

                CMatrix4f trans;
                MatrixTranslation(trans, CVec3f(-150, 0, 0));

                CMatrix4f model;
                MatrixMultiply(trans, model_1, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                greng::CTexture* texts[6] = { tex7, tex7normal,
                                              tex7, tex7normal,
                                              tex7, tex7normal, };

                for (unsigned int i = 0; i < 3; i++) {
                    GetGrengSystems().GetRenderer().RenderMesh(
                        mesh4, i, &texts[i * 2], 2, shaderProgram4, &model,
                        nullptr, &model_view,
                        &GetCamera().GetProjectionMatrix(), &light1, nullptr,
                        &GetCamera().GetPos().Get());
                }
            }

            GetGrengSystems().GetRenderer().DrawPoint(
                GetCamera(), light1.position, 10, CColor4f(1, 1, 1, 1), false);
        }

        void CTest5::SetupMeshes() {
            mesh1 = GetGrengSystems().GetMeshManager().CreateMeshCube();
            mesh2 = GetGrengSystems().GetMeshManager().CreateMeshQuad();
            mesh3 = GetGrengSystems().GetMeshManager().CreateMeshFromObjFile(
                "assets/mt.obj");
            mesh4 = GetGrengSystems().GetMeshManager().CreateMeshFromObjFile(
                "assets/RB-BumbleBee.obj");

            GetGrengSystems().GetMeshManager().ComputeNormals(mesh3);
            GetGrengSystems().GetMeshManager().ComputeTangentSpace(mesh3);
            GetGrengSystems().GetMeshManager().ComputeNormals(mesh4);

            CMatrix4f s;
            MatrixScale(s, CVec3f(0.1));

            CMatrix4f rx;
            MatrixRotationX(rx, -M_PI / 2.0);

            CMatrix4f ry;
            MatrixRotationY(ry, -M_PI / 2.0);

            CMatrix4f rxy;
            MatrixMultiply(ry, rx, rxy);

            MatrixMultiply(rxy, s, mesh3ConstMatrix);
        }

        void CTest5::SetupTextures() {
            tex1 = GetGrengSystems().GetTextureManager().CreateTextureDummy();
            tex2 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex3 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex3normal =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/floor/normal.png");
            tex4 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex4normal =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/floor/normal.png");
            tex5 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex5normal =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/floor/normal.png");
            tex6 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex7 = GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex7normal =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/floor/normal.png");
        }

        void CTest5::SetupShaders() {
            vertexShader1 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader1.120.vs");
            fragmentShader1 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader1.120.fs");
            shaderProgram1 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader1, fragmentShader1);
            vertexShader2 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader2.120.vs");
            fragmentShader2 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader2.120.fs");
            shaderProgram2 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader2, fragmentShader2);
            vertexShader3 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader3.120.vs");
            fragmentShader3 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader3.120.fs");
            shaderProgram3 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader3, fragmentShader3);
            vertexShader4 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader4.120.vs");
            fragmentShader4 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader4.120.fs");
            shaderProgram4 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader4, fragmentShader4);
        }

        void CTest5::SetupProcessors() {}

        void CTest5::SetupLights() { light1.position.Set(-50, 100, 0); }

    } // namespace test

} // namespace drash
