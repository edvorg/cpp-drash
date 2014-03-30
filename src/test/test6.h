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

#ifndef DRASH_TEST_TEST6_H
#define DRASH_TEST_TEST6_H

#include "test1.h"
#include "../greng/pointlight.h"

namespace drash {

    namespace test {

        class CTest6 : public CTest1 {
        public:
            CTest6(greng::CGrengSystemsSet& greng);

            virtual void Step(double _dt) override;
            virtual void Render() override;

        private:
            bool InitCamera();
            bool InitLevel();
            bool InitPlayer();
            bool InitLight();
            bool InitProcessors();
            bool InitTarget();

            CPlayer* player1 = nullptr;
            greng::CMesh* player1Mesh = nullptr;
            greng::CTexture* player1Texture = nullptr;
            greng::CVertexShader* player1VertexShader = nullptr;
            greng::CFragmentShader* player1FragmentShader = nullptr;
            greng::CShaderProgram* player1ShaderProgram = nullptr;
            CVec2f player1MeshDir = CVec2f(1, 0);
            CVec3f player1OldPos;

            float angle = 0;

            bool followPlayer = true;

            float targetCreateTimer = 0;
            bool targetDestroyed = true;
            CSceneObjectGeometry* targetGeometry = nullptr;

            greng::CPointLight light1;
        };

    } // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST6_H
