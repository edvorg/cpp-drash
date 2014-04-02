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

        Test12::Test12(greng::Greng& greng) : App(greng) {
            camera = GetGreng().GetCameraManager().CreateCamera({});
            camera->GetDepthOfView() = 100;
            camera->GetPos() = { 0, 0, 10 };
            camera->GetOrtho() = true;
            camera->GetOrthoSize() = { 1, 1 };            
            texture = GetGreng().GetTextureManager().CreateTextureFromFile(
                "assets/space/diffuse.png");
            renderTarget = GetGreng().GetTextureManager().CreateTexture(
                GetGreng().GetViewport().GetSize());
            frameBuffer = GetGreng().GetFrameBufferManager().CreateFrameBuffer(
                *renderTarget);

            quad = GetGreng().GetMeshManager().CreateMeshQuad();
            vertexShader1 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader8.120.vs");
            fragmentShader1 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader8.120.fs");
            shaderProgram1 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader1, fragmentShader1);

            quad = GetGreng().GetMeshManager().CreateMeshQuad();
            vertexShader2 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader9.120.vs");
            fragmentShader2 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader9.120.fs");
            shaderProgram2 = GetGreng().GetShaderProgramManager().CreateProgram(
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

            GetGreng().GetRenderer().Clear(frameBuffer, { 0.5, 0.5, 0.5, 1}); 
            GetGreng().GetRenderer().RenderMesh(
                quad, 0, &texture, 1, shaderProgram1, {}, {},
                &camera->GetViewMatrix().getValue(), &camera->GetProjectionMatrix().getValue(), {},
                {}, {}, frameBuffer);

            GetGreng().GetRenderer().RenderMesh(
                quad, 0, &renderTarget, 1, shaderProgram2, {}, {},
                &camera->GetViewMatrix().getValue(), &camera->GetProjectionMatrix().getValue(),
                &light);
        }

    } // namespace test

} // namespace drash
