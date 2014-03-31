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

        Test5::Test5(greng::Greng& greng) : Test3(greng) {
            SetupProcessors();
            SetupMeshes();
            SetupTextures();
            SetupShaders();
            SetupLights();

            light1.position.Set(0, 50, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void Test5::Step(double _dt) {
            Test3::Step(_dt);

            angle += 0.5 * _dt;

            while (angle > M_PI * 2.0) {
                angle -= M_PI * 2.0;
            }

            light1.position.x = 200 * sin(angle);
            light1.position.y = 50;
            light1.position.z = 200 * cos(angle);
        }

        void Test5::Render() {
            Test3::Render();

            if (mesh1 != nullptr) {
                Matrix4f r;
                MatrixRotationZ(r, angle);

                Matrix4f s;
                MatrixScale(s, Vec3f(10));

                Matrix4f rot;
                MatrixMultiply(r, s, rot);

                Matrix4f transl;
                MatrixTranslation(transl, Vec3f(-100, 30, 0));

                Matrix4f model;
                MatrixMultiply(transl, rot, model);

                Matrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                GetGreng().GetRenderer().RenderMesh(
                    mesh1, 0, &tex6, 1, shaderProgram2, &model, nullptr,
                    &model_view, &GetCamera().GetProjectionMatrix(), &light1,
                    {}, {}, frameBuffer);
            }

            if (mesh2 != nullptr) {
                Matrix4f r;
                MatrixRotationZ(r, -angle);

                Matrix4f s;
                MatrixScale(s, Vec3f(10));

                Matrix4f rot;
                MatrixMultiply(r, s, rot);

                Matrix4f transl;
                MatrixTranslation(transl, Vec3f(100, 30, 0));

                Matrix4f model;
                MatrixMultiply(transl, rot, model);

                Matrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                GetGreng().GetRenderer().RenderMesh(
                    mesh2, 0, &tex2, 1, shaderProgram2, &model, nullptr,
                    &model_view, &GetCamera().GetProjectionMatrix(), &light1,
                    {}, {}, frameBuffer);
            }

            if (mesh3 != nullptr) {
                Matrix4f rangle;
                MatrixRotationY(rangle, 0);

                Matrix4f model;
                MatrixMultiply(rangle, mesh3ConstMatrix, model);

                Matrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                greng::Texture* texts[6] = {
                    tex4, tex4normal, tex3, tex3normal, tex5, tex5normal
                };

                for (unsigned int i = 0; i < 3; i++) {
                    GetGreng().GetRenderer().RenderMesh(
                        mesh3, i, &texts[i * 2], 2, shaderProgram4, &model,
                        nullptr, &model_view,
                        &GetCamera().GetProjectionMatrix(), &light1, nullptr,
                        &GetCamera().GetPos().Get(), frameBuffer);
                }
            }

            if (mesh4 != nullptr) {
                Matrix4f rangle;
                MatrixRotationY(rangle, 0);

                Matrix4f model_1;
                MatrixMultiply(rangle, mesh3ConstMatrix, model_1);

                Matrix4f trans;
                MatrixTranslation(trans, Vec3f(-150, 0, 0));

                Matrix4f model;
                MatrixMultiply(trans, model_1, model);

                Matrix4f model_view;
                MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

                greng::Texture* texts[6] = {
                    tex7, tex7normal, tex7, tex7normal, tex7, tex7normal,
                };

                for (unsigned int i = 0; i < 3; i++) {
                    GetGreng().GetRenderer().RenderMesh(
                        mesh4, i, &texts[i * 2], 2, shaderProgram4, &model,
                        nullptr, &model_view,
                        &GetCamera().GetProjectionMatrix(), &light1, nullptr,
                        &GetCamera().GetPos().Get(), frameBuffer);
                }
            }

            GetGreng().GetRenderer().DrawPoint(GetCamera(), light1.position, 10,
                                               Color4f(1, 1, 1, 1), false);
        }
        
        void Test5::SetupMeshes() {
            mesh1 = GetGreng().GetMeshManager().CreateMeshCube();
            mesh2 = GetGreng().GetMeshManager().CreateMeshQuad();
            mesh3 = GetGreng().GetMeshManager().CreateMeshFromObjFile(
                "assets/mt.obj");
            mesh4 = GetGreng().GetMeshManager().CreateMeshFromObjFile(
                "assets/RB-BumbleBee.obj");

            GetGreng().GetMeshManager().ComputeNormals(mesh3);
            GetGreng().GetMeshManager().ComputeTangentSpace(mesh3);
            GetGreng().GetMeshManager().ComputeNormals(mesh4);

            Matrix4f s;
            MatrixScale(s, Vec3f(0.1));

            Matrix4f rx;
            MatrixRotationX(rx, -M_PI / 2.0);

            Matrix4f ry;
            MatrixRotationY(ry, -M_PI / 2.0);

            Matrix4f rxy;
            MatrixMultiply(ry, rx, rxy);

            MatrixMultiply(rxy, s, mesh3ConstMatrix);
        }

        void Test5::SetupTextures() {
            tex1 = GetGreng().GetTextureManager().CreateTextureDummy();
            tex2 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex3 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex3normal = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/normal.png");
            tex4 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex4normal = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/normal.png");
            tex5 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex5normal = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/normal.png");
            tex6 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex7 = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
            tex7normal = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/floor/normal.png");
        }

        void Test5::SetupShaders() {
            vertexShader1 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader1.120.vs");
            fragmentShader1 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader1.120.fs");
            shaderProgram1 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader1, fragmentShader1);
            vertexShader2 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader2.120.vs");
            fragmentShader2 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader2.120.fs");
            shaderProgram2 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader2, fragmentShader2);
            vertexShader3 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader3.120.vs");
            fragmentShader3 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader3.120.fs");
            shaderProgram3 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader3, fragmentShader3);
            vertexShader4 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader4.120.vs");
            fragmentShader4 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader4.120.fs");
            shaderProgram4 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader4, fragmentShader4);
        }

        void Test5::SetupProcessors() {}

        void Test5::SetupLights() { light1.position.Set(-50, 100, 0); }

    } // namespace test

} // namespace drash
