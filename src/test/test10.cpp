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

#include "test10.h"

#include "../greng/camera.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"

namespace drash {

    namespace test {

        Test10::Test10(greng::Greng& greng) : App(greng) {
            InitCamera();
            InitObjects();
            InitLights();
            InitProcessors();
            GetDebugRenderer().SetTexCoordsScale(0.2);
        }

        void Test10::Step(double _dt) {
            App::Step(_dt);

            lightPoint1.position = lamp1Object->GetPos();
            spotLight.position = lamp1Object->GetPos();

            float angle = lamp1Object->GetAngle() + M_PI * 1.5;

            spotLight.direction.Set(cos(angle), sin(angle), 0);
        }

        void Test10::Render() {
            App::Render();

            Vec3f p1(lamp1Object->GetWorldPoint(Vec2f(0, 0.5)),
                     lamp1Object->GetPosZ());
            GetGreng().GetRenderer().DrawLine(*camera, p1,
                                              wallTopObject->GetPos(), 2,
                                              Color4f(0, 0, 0, 1), true);
        }

        bool Test10::InitCamera() {
            greng::CameraParams params;
            params.pos.Set(0, 2, 20);
            params.rotation.x = -M_PI / 18.0;

            greng::Camera* cam =
                GetGreng().GetCameraManager().CreateCamera(params);

            if (cam == nullptr) {
                return false;
            }

            GetDebugRenderer().SetCamera(cam);

            camera = cam;

            return true;
        }

        bool Test10::InitObjects() {
            SceneObjectGeometry* g1 =
                GetGeometryManager().CreateGeometry("wall_back");
            SceneObjectGeometry* g2 =
                GetGeometryManager().CreateGeometry("wall_top");
            SceneObjectGeometry* g3 =
                GetGeometryManager().CreateGeometry("wall_bottom");
            SceneObjectGeometry* g4 =
                GetGeometryManager().CreateGeometry("wall");
            SceneObjectGeometry* g5 =
                GetGeometryManager().CreateGeometry("lamp");

            if (g1 == nullptr || g2 == nullptr || g3 == nullptr ||
                g4 == nullptr || g5 == nullptr) {
                return false;
            }

            g1->figures.resize(1);
            g1->figures[0].depth = 0.5;
            g1->figures[0].vertices.push_back(Vec2f(-10, 5));
            g1->figures[0].vertices.push_back(Vec2f(-10, -5));
            g1->figures[0].vertices.push_back(Vec2f(10, -5));
            g1->figures[0].vertices.push_back(Vec2f(10, 5));

            g2->figures.resize(1);
            g2->figures[0].depth = 8;
            g2->figures[0].vertices.push_back(Vec2f(-10, 0.2));
            g2->figures[0].vertices.push_back(Vec2f(-10, -0.2));
            g2->figures[0].vertices.push_back(Vec2f(10, -0.2));
            g2->figures[0].vertices.push_back(Vec2f(10, 0.2));

            g3->figures.resize(1);
            g3->figures[0].depth = 3;
            g3->figures[0].vertices.push_back(Vec2f(-10, 0.2));
            g3->figures[0].vertices.push_back(Vec2f(-10, -0.2));
            g3->figures[0].vertices.push_back(Vec2f(10, -0.2));
            g3->figures[0].vertices.push_back(Vec2f(10, 0.2));

            g4->figures.resize(1);
            g4->figures[0].depth = 3;
            g4->figures[0].vertices.push_back(Vec2f(-0.4, 4.0));
            g4->figures[0].vertices.push_back(Vec2f(-0.4, -4.0));
            g4->figures[0].vertices.push_back(Vec2f(0.4, -4.0));
            g4->figures[0].vertices.push_back(Vec2f(0.4, 4.0));

            g5->figures.resize(1);
            g5->figures[0].depth = 0.4;
            g5->figures[0].mass = 0.1;
            g5->figures[0].vertices.push_back(Vec2f(-0.2, 0.5));
            g5->figures[0].vertices.push_back(Vec2f(-0.2, -0.5));
            g5->figures[0].vertices.push_back(Vec2f(0.2, -0.5));
            g5->figures[0].vertices.push_back(Vec2f(0.2, 0.5));

            SceneObjectParams p1;
            p1.pos.Set(0, 0, -5);
            p1.dynamic = false;
            GetGeometryManager().CreateSceneObject("wall_back", p1);

            SceneObjectParams p2;
            p2.pos.Set(0, 5, 0);
            p2.dynamic = true;
            SceneObject* o2 =
                GetGeometryManager().CreateSceneObject("wall_top", p2);

            SceneObjectParams p3;
            p3.pos.Set(0, -4, 0);
            p3.dynamic = false;
            GetGeometryManager().CreateSceneObject("wall_bottom", p3);

            SceneObjectParams p4;
            p4.pos.Set(-9, 0, 0);
            p4.dynamic = true;
            GetGeometryManager().CreateSceneObject("wall", p4);

            SceneObjectParams p5;
            p5.pos.Set(9, 0, 0);
            p5.dynamic = true;
            GetGeometryManager().CreateSceneObject("wall", p5);

            SceneObjectParams p6;
            p6.pos.Set(-3, 3, 0);
            p6.dynamic = true;
            SceneObject* o6 =
                GetGeometryManager().CreateSceneObject("lamp", p6);

            Vec3f anchor1(0, 0, 0);
            anchor1.y += 0.5;
            Vec3f anchor2(0, 0, 0);
            GetScene().CreateJointRope(o6, o2, anchor1, anchor2, 3);

            lamp1Object = o6;
            wallTopObject = o2;

            return true;
        }

        bool Test10::InitLights() {
            lightPoint1.position.Set(0, 0, 0);

            GetDebugRenderer().SetLight(nullptr);
            GetDebugRenderer().SetSpotLight(&spotLight);

            return true;
        }

        bool Test10::InitProcessors() {
            GetEventSystem().SetProcessor(
                "C-q", AppEventProcessor([this]() { this->Quit(); }, []() {},
                                         []() {}));

            return true;
        }

    } // namespace test

} // namespace drash
