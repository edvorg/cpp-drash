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

#ifndef DRASH_TEST_TEST6_H
#define DRASH_TEST_TEST6_H

#include "test1.h"
#include "../greng/pointlight.h"

namespace drash
{

namespace test
{

class CTest6 : public CTest1
{
public:
    CTest6() = default;

protected:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;

private:
    bool InitPlayer();
    bool InitLight();
    bool InitProcessors();

    CPlayer *mPlayer1 = nullptr;
    greng::CMesh *mPlayer1Mesh = nullptr;
    greng::CTexture *mPlayer1Texture = nullptr;
    greng::CVertexShader *mPlayer1VertexShader = nullptr;
    greng::CFragmentShader *mPlayer1FragmentShader = nullptr;
    greng::CShaderProgram * mPlayer1ShaderProgram = nullptr;
    CVec2f mPlayer1MeshDir = CVec2f(1, 0);
    CVec3f mPlayer1OldPos;

    float mAngle = 0;

    greng::CPointLight mLight1;
};

} // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST6_H
