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

#ifndef DRASH_TEST_TEST5_H
#define DRASH_TEST_TEST5_H

#include "test3.h"
#include "../greng/pointlight.h"
#include "../misc/matrix4.h"

namespace drash {

    namespace test {

        class Test5 : public Test3 {
        public:
            Test5(greng::Greng& greng);

        protected:
            virtual void Step(double _dt) override;
            virtual void Render() override;
            void SetFrameBuffer(greng::FrameBuffer* _frameBuffer) {
                GetDebugRenderer().SetFrameBuffer(frameBuffer = _frameBuffer);
            }
            void SetShaderProgram(greng::ShaderProgram* _program) {
                GetDebugRenderer().SetShaderProgram(shaderProgram = _program);
            }

        protected:
        private:
            void SetupMeshes();
            void SetupTextures();
            void SetupShaders();
            void SetupProcessors();
            void SetupLights();

            greng::Mesh* mesh1;
            greng::Mesh* mesh2;
            greng::Mesh* mesh3;
            greng::Mesh* mesh4;
            Matrix4f mesh3ConstMatrix;
            greng::Texture* tex1;
            greng::Texture* tex2;
            greng::Texture* tex3;
            greng::Texture* tex3normal;
            greng::Texture* tex4;
            greng::Texture* tex4normal;
            greng::Texture* tex5;
            greng::Texture* tex5normal;
            greng::Texture* tex6;
            greng::Texture* tex7;
            greng::Texture* tex7normal;
            greng::ShaderProgram* shaderProgram;
            greng::VertexShader* vertexShader1;
            greng::FragmentShader* fragmentShader1;
            greng::ShaderProgram* shaderProgram1;
            greng::VertexShader* vertexShader2;
            greng::FragmentShader* fragmentShader2;
            greng::ShaderProgram* shaderProgram2;
            greng::VertexShader* vertexShader3;
            greng::FragmentShader* fragmentShader3;
            greng::ShaderProgram* shaderProgram3;
            greng::VertexShader* vertexShader4;
            greng::FragmentShader* fragmentShader4;
            greng::ShaderProgram* shaderProgram4;
            greng::FrameBuffer* frameBuffer;

            double angle = 0.0;

            greng::PointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST5_H
