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

#include "test13.hpp"

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

        Test13::Test13(greng::Greng& greng) : Test5(greng) {            
            camera = GetGreng().GetCameraManager().CreateCamera({});
            camera->GetDepthOfView() = 100;
            camera->GetPos() = { 0, 0, 10 };
            camera->SetOrtho(true);
            camera->GetOrthoSize() = { 1, 1 };

            auto size = GetGreng().GetViewport().GetSize();
            renderTarget = GetGreng().GetTextureManager().CreateTexture(size);
            renderTargetDepth =
                GetGreng().GetTextureManager().CreateTextureDepth(size);
            frameBuffer = GetGreng().GetFrameBufferManager().CreateFrameBuffer(
                *renderTarget, *renderTargetDepth);

            quad = GetGreng().GetMeshManager().CreateMeshQuad();

            quad = GetGreng().GetMeshManager().CreateMeshQuad();
            vertexShader2 =
                GetGreng().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader9.120.vs");
            fragmentShader2 =
                GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
                    "shaders/shader9.120.fs");
            shaderProgram2 = GetGreng().GetShaderProgramManager().CreateProgram(
                vertexShader2, fragmentShader2);

            GetEventSystem().SetProcessor(
                "C-q", AppEventProcessor([this] { Quit(); }));

            Test5::SetFrameBuffer(frameBuffer);
        }

        void Test13::Step(double _dt) {
            Test5::Step(_dt);

            static auto angle = 0.0f;
            angle += _dt;
            light.position.x = sin(angle) * 0.05;
        }

        void Test13::Render() {
            GetGreng().GetRenderer().Clear(frameBuffer, { 0.5, 0.5, 0.5, 1 });

            Test5::Render();            

            GetGreng().GetRenderer().RenderMesh(
                quad, 0, &renderTarget, 1, shaderProgram2, {}, {},
                &camera->GetViewMatrix(), &camera->GetProjectionMatrix(),
                &light);
        }

    } // namespace test

} // namespace drash
