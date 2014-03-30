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

        protected:
        private:
            void SetupMeshes();
            void SetupTextures();
            void SetupShaders();
            void SetupProcessors();
            void SetupLights();

            greng::Mesh* mesh1 = nullptr;
            greng::Mesh* mesh2 = nullptr;
            greng::Mesh* mesh3 = nullptr;
            greng::Mesh* mesh4 = nullptr;
            Matrix4f mesh3ConstMatrix;
            greng::Texture* tex1 = nullptr;
            greng::Texture* tex2 = nullptr;
            greng::Texture* tex3 = nullptr;
            greng::Texture* tex3normal = nullptr;
            greng::Texture* tex4 = nullptr;
            greng::Texture* tex4normal = nullptr;
            greng::Texture* tex5 = nullptr;
            greng::Texture* tex5normal = nullptr;
            greng::Texture* tex6 = nullptr;
            greng::Texture* tex7 = nullptr;
            greng::Texture* tex7normal = nullptr;
            greng::VertexShader* vertexShader1 = nullptr;
            greng::FragmentShader* fragmentShader1 = nullptr;
            greng::ShaderProgram* shaderProgram1 = nullptr;
            greng::VertexShader* vertexShader2 = nullptr;
            greng::FragmentShader* fragmentShader2 = nullptr;
            greng::ShaderProgram* shaderProgram2 = nullptr;
            greng::VertexShader* vertexShader3 = nullptr;
            greng::FragmentShader* fragmentShader3 = nullptr;
            greng::ShaderProgram* shaderProgram3 = nullptr;
            greng::VertexShader* vertexShader4 = nullptr;
            greng::FragmentShader* fragmentShader4 = nullptr;
            greng::ShaderProgram* shaderProgram4 = nullptr;
            greng::FrameBuffer* frameBuffer;

            double angle = 0.0;

            greng::PointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST5_H
