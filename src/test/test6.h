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

        class Test6 : public Test1 {
        public:
            Test6(greng::Greng& greng);

            virtual void Step(double _dt) override;
            virtual void Render() override;

        private:
            bool InitCamera();
            bool InitLevel();
            bool InitPlayer();
            bool InitLight();
            bool InitProcessors();
            bool InitTarget();

            Player* player1 = nullptr;
            greng::Mesh* player1Mesh = nullptr;
            greng::Texture* player1Texture = nullptr;
            greng::VertexShader* player1VertexShader = nullptr;
            greng::FragmentShader* player1FragmentShader = nullptr;
            greng::ShaderProgram* player1ShaderProgram = nullptr;
            Vec2f player1MeshDir = Vec2f(1, 0);
            Vec3f player1OldPos;

            float angle = 0;

            bool followPlayer = true;

            float targetCreateTimer = 0;
            bool targetDestroyed = true;
            SceneObjectGeometry* targetGeometry = nullptr;

            greng::PointLight light1;
        };

    } // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST6_H
