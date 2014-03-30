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

#include "test12.hpp"

#include "../greng/camera.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"
#include "../greng/framebuffer.hpp"
#include <sstream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <SDL/SDL_opengl.h>

namespace drash {

    namespace test {

        Test12::Test12(greng::GrengSystemsSet& greng) : App(greng) {
            camera = GetGrengSystems().GetCameraManager().CreateCamera({});
            camera->GetDepthOfView() = 100;
            camera->GetPos() = { 0, 0, 10 };
            camera->SetOrtho(true);
            camera->GetOrthoWidth() = 1;
            camera->GetOrthoHeight() = 1;
            texture =
                GetGrengSystems().GetTextureManager().CreateTextureFromFile(
                    "assets/space/diffuse.png");
            renderTarget = GetGrengSystems().GetTextureManager().CreateTexture(
                GetGrengSystems().GetViewport().GetSize());
            frameBuffer =
                GetGrengSystems().GetFrameBufferManager().CreateFrameBuffer(
                    *renderTarget);

            quad = GetGrengSystems().GetMeshManager().CreateMeshQuad();
            vertexShader1 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader8.120.vs");
            fragmentShader1 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader8.120.fs");
            shaderProgram1 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader1, fragmentShader1);

            quad = GetGrengSystems().GetMeshManager().CreateMeshQuad();
            vertexShader2 =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader9.120.vs");
            fragmentShader2 =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader9.120.fs");
            shaderProgram2 =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    vertexShader2, fragmentShader2);

            GetDebugRenderer().SetCamera(camera);
            GetDebugRenderer().SetLight(&light);
            GetDebugRenderer().SetTexCoordsScale(0.3);

            GetEventSystem().SetProcessor(
                "C-q", AppEventProcessor([this] { Quit(); }));
        }

        void Test12::Step(double _dt) {
            App::Step(_dt);

            static auto angle = 0.0f;
            angle += _dt;
            light.position.x = sin(angle);
        }

        void Test12::Render() {
            App::Render();

            GetGrengSystems().GetRenderer().RenderMesh(
                quad, 0, &texture, 1, shaderProgram1, {}, {},
                &camera->GetViewMatrix(), &camera->GetProjectionMatrix(), {},
                {}, {}, frameBuffer);

            GetGrengSystems().GetRenderer().RenderMesh(
                quad, 0, &renderTarget, 1, shaderProgram2, {}, {},
                &camera->GetViewMatrix(), &camera->GetProjectionMatrix(),
                &light);
        }

    } // namespace test

} // namespace drash
