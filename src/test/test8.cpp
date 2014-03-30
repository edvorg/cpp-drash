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

#include "test8.h"

#include "../levelmanager/leveldesc.h"
#include "../scene/figure.h"
#include "../greng/camera.h"

namespace drash {

    namespace test {

        Test8::Test8(greng::Greng& greng) : App(greng) {
            InitUI();
            InitLevels();
            InitCamera();
            InitLights();
            GetGeometryManager().Load();
        }

        void Test8::Render() { App::Render(); }

        Test8::~Test8() {
            GetGeometryManager().Store();
            level1->Store("level1");
            level2->Store("level2");
        }

        bool Test8::InitUI() {
            GetUISystem().SetWidth(1024);

            button1.Connect(&GetUISystem());
            button1.SetPos(Vec2i(5, 30));
            button1.SetSize(Vec2ui(100, 20));

            button1.SetClickHandler([this]() {
                if (level1 != nullptr) {
                    GetLevelManager().StartLevel(level1);
                }
            });

            button2.Connect(&GetUISystem());
            button2.SetPos(Vec2i(5, 5));
            button2.SetSize(Vec2ui(100, 20));

            button2.SetClickHandler([this]() {
                if (level2 != nullptr) {
                    GetLevelManager().StartLevel(level2);
                }
            });

            return true;
        }

        bool Test8::InitLevels() {
            level1 = GetLevelManager().CreateLevel();
            level2 = GetLevelManager().CreateLevel();

            level1->Load("level1");
            level2->Load("level2");
            return true;
            if (level1 == nullptr || level2 == nullptr) {
                return false;
            }

            SceneObjectGeometry* g1 =
                GetGeometryManager().CreateGeometry("object1_geometry");
            SceneObjectGeometry* g2 =
                GetGeometryManager().CreateGeometry("object2_geometry");

            if (g1 == nullptr || g2 == nullptr) {
                return false;
            }

            g1->figures.resize(1);
            g1->figures[0].depth = 1;
            g1->figures[0].vertices.push_back(Vec2f(-1, 0));
            g1->figures[0].vertices.push_back(Vec2f(1, 0));
            g1->figures[0].vertices.push_back(Vec2f(0, 1));

            g2->figures.resize(1);
            g2->figures[0].depth = 1;
            g2->figures[0].vertices.push_back(Vec2f(-1, -1));
            g2->figures[0].vertices.push_back(Vec2f(1, -1));
            g2->figures[0].vertices.push_back(Vec2f(1, 1));
            g2->figures[0].vertices.push_back(Vec2f(-1, 1));

            LevelObjectDesc* p1 =
                level1->AddObject("object1_geometry", "obj1");
            LevelObjectDesc* p2 =
                level1->AddObject("object2_geometry", "obj2");

            if (p1 == nullptr || p2 == nullptr) {
                return false;
            }

            p2->params.pos.Set(3, 0, 0);

            LevelObjectDesc* p3 =
                level2->AddObject("object1_geometry", "obj1");
            LevelObjectDesc* p4 =
                level2->AddObject("object2_geometry", "obj2");

            if (p3 == nullptr || p4 == nullptr) {
                return false;
            }

            p3->params.pos.Set(3, 0, 0);

            return true;
        }

        bool Test8::InitCamera() {
            greng::CameraParams p;
            p.pos.Set(0, 0, 10);
            p.fov = M_PI / 6.0;
            auto c = GetGreng().GetCameraManager().CreateCamera(p);
            GetDebugRenderer().SetCamera(c);

            return true;
        }

        bool Test8::InitLights() {
            light1.position.Set(0, 10, 0);
            GetDebugRenderer().SetLight(&light1);
            return true;
        }

    } // namespace test

} // namespace drash
