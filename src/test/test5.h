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

        class CTest5 : public CTest3 {
          public:
            CTest5();

          protected:
            virtual void Step(double _dt) override;
            virtual void Render() override;

          protected:
          private:
            void SetupMeshes();
            void SetupTextures();
            void SetupShaders();
            void SetupProcessors();
            void SetupLights();

            greng::CMesh* mesh1 = nullptr;
            greng::CMesh* mesh2 = nullptr;
            greng::CMesh* mesh3 = nullptr;
            greng::CMesh* mesh4 = nullptr;
            CMatrix4f mesh3ConstMatrix;
            greng::CTexture* tex1 = nullptr;
            greng::CTexture* tex2 = nullptr;
            greng::CTexture* tex3 = nullptr;
            greng::CTexture* tex3normal = nullptr;
            greng::CTexture* tex4 = nullptr;
            greng::CTexture* tex4normal = nullptr;
            greng::CTexture* tex5 = nullptr;
            greng::CTexture* tex5normal = nullptr;
            greng::CTexture* tex6 = nullptr;
            greng::CTexture* tex7 = nullptr;
            greng::CTexture* tex7normal = nullptr;
            greng::CVertexShader* vertexShader1 = nullptr;
            greng::CFragmentShader* fragmentShader1 = nullptr;
            greng::CShaderProgram* shaderProgram1 = nullptr;
            greng::CVertexShader* vertexShader2 = nullptr;
            greng::CFragmentShader* fragmentShader2 = nullptr;
            greng::CShaderProgram* shaderProgram2 = nullptr;
            greng::CVertexShader* vertexShader3 = nullptr;
            greng::CFragmentShader* fragmentShader3 = nullptr;
            greng::CShaderProgram* shaderProgram3 = nullptr;
            greng::CVertexShader* vertexShader4 = nullptr;
            greng::CFragmentShader* fragmentShader4 = nullptr;
            greng::CShaderProgram* shaderProgram4 = nullptr;

            double angle = 0.0;

            greng::CPointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST5_H
