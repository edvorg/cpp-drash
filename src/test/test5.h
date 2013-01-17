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

#ifndef DRASH_TEST_TEST5_H
#define DRASH_TEST_TEST5_H

#include "test3.h"
#include "../greng/pointlight.h"

namespace drash
{

namespace test
{

class CTest5 : public CTest3
{
public:
    CTest5() = default;

protected:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;

protected:
private:
    void SetupMeshes();
    void SetupTextures();
    void SetupShaders();
    void SetupProcessors();
    void SetupLights();

    greng::CMesh *mMesh1 = nullptr;
    greng::CMesh *mMesh2 = nullptr;
    greng::CMesh *mMesh3 = nullptr;
    greng::CMesh *mMesh4 = nullptr;
    CMatrix4f mMesh3ConstMatrix;
    greng::CTexture *mTex1 = nullptr;
    greng::CTexture *mTex2 = nullptr;
    greng::CTexture *mTex3 = nullptr;
    greng::CTexture *mTex3normal = nullptr;
    greng::CTexture *mTex4 = nullptr;
    greng::CTexture *mTex4normal = nullptr;
    greng::CTexture *mTex5 = nullptr;
    greng::CTexture *mTex5normal = nullptr;
    greng::CTexture *mTex6 = nullptr;
    greng::CTexture *mTex7 = nullptr;
    greng::CTexture *mTex7normal = nullptr;
    greng::CVertexShader *mVertexShader1 = nullptr;
    greng::CFragmentShader *mFragmentShader1 = nullptr;
    greng::CShaderProgram *mShaderProgram1 = nullptr;
    greng::CVertexShader *mVertexShader2 = nullptr;
    greng::CFragmentShader *mFragmentShader2 = nullptr;
    greng::CShaderProgram *mShaderProgram2 = nullptr;
    greng::CVertexShader *mVertexShader3 = nullptr;
    greng::CFragmentShader *mFragmentShader3 = nullptr;
    greng::CShaderProgram *mShaderProgram3 = nullptr;

    double mAngle = 0.0;

    greng::CPointLight mLight1;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST5_H
