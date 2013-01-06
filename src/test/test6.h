// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#ifndef DRASH_TEST_TEST6_H
#define DRASH_TEST_TEST6_H

#include "../app/app.h"
#include "../greng/pointlight.h"

namespace drash
{

namespace test
{

class CTest6 : public CApp
{
public:
    CTest6() = default;

protected:
    bool Init() override;
    void Step(double _dt) override;
    void Render() override;

private:
    bool InitPlayer();
    bool InitLight();
    bool InitProcessors();

    unsigned int mPlayer1Id = 0;
    greng::CMesh *mPlayer1Mesh = nullptr;
    greng::CTexture *mPlayer1Texture = nullptr;
    greng::CVertexShader *mPlayer1VertexShader = nullptr;
    greng::CFragmentShader *mPlayer1FragmentShader = nullptr;
    greng::CShaderProgram * mPlayer1ShaderProgram = nullptr;
    float mPlayer1Angle = 0.0f;
    CVec3f mPlayer1OldPos;

    greng::CPointLight mPointLight1;

    float mAngle = 0;
};

} // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST6_H
