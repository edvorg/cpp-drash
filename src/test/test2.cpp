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

#include "test2.h"

#include "../scene/scene.h"
#include "../app/appeventprocessor.h"
#include "../greng/camera.h"
#include "../scene/sceneobject.h"
#include "../players/player.h"
#include "../scene/figure.h"
#include "../misc/plane.h"

namespace drash {

    namespace test {

        Test2::Test2(greng::GrengSystemsSet& greng) : App(greng) {
            greng::CameraParams cp;
            cp.pos.Set(0, 0, 300);
            camera = GetGrengSystems().GetCameraManager().CreateCamera(cp);

            GetDebugRenderer().SetCamera(camera);

            SetProcessors();

            SceneObjectGeometry g;
            g.figures.resize(1);
            g.figures[0].vertices.push_back(Vec2f(-300.0f, 5.0f));
            g.figures[0].vertices.push_back(Vec2f(-300.0f, -5.0f));
            g.figures[0].vertices.push_back(Vec2f(300.0f, -5.0f));
            g.figures[0].vertices.push_back(Vec2f(300.0f, 5.0f));
            g.figures[0].depth = 5;

            SceneObjectParams p;
            p.pos.y = -25;
            p.dynamic = false;
            GetScene().CreateObject(g, p);

            SceneObjectGeometry player_geometry;
            player_geometry.figures.resize(1);
            player_geometry.figures[0].vertices.push_back(Vec2f(-2, -5));
            player_geometry.figures[0].vertices.push_back(Vec2f(2, -5));
            player_geometry.figures[0].vertices.push_back(Vec2f(2, 5));
            player_geometry.figures[0].vertices.push_back(Vec2f(-2, 5));
            player_geometry.figures[0].depth = 1;

            PlayerParams player;
            player.sceneObjectParams.pos.Set(0, -20, 0);

            GetPlayersSystem().CreatePlayer(player_geometry, player);

            SceneObjectGeometry tg;
            tg.figures.resize(1);
            tg.figures[0].vertices.push_back(Vec2f(-10, -5));
            tg.figures[0].vertices.push_back(Vec2f(10, -5));
            tg.figures[0].vertices.push_back(Vec2f(10, 5));
            tg.figures[0].vertices.push_back(Vec2f(-10, 5));
            tg.figures[0].friction = 0.5;
            tg.figures[0].mass = 1;
            tg.figures[0].depth = 1;
            SceneObjectParams targetForFire;
            targetForFire.pos.Set(-20, 0, 0);
            for (int i = 0; i < 10; i++) {
                GetScene().CreateObject(tg, targetForFire);
                targetForFire.pos.Set(-20, 20 + i * 20, 0);
            }

            light1.position.Set(0, 30, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void Test2::SetProcessors() {
            auto t = GetGeometryManager().CreateGeometry("lambda_test");
            t->figures.resize(1);
            t->figures[0].vertices.push_back(Vec2f(-10, -10));
            t->figures[0].vertices.push_back(Vec2f(10, -10));
            t->figures[0].vertices.push_back(Vec2f(0, 10));
            t->figures[0].depth = 3;

            GetEventSystem().SetProcessor(
                "C-S-f",
                AppEventProcessor([this, t]() {
                    Plane plane;
                    plane.SetPoint(Vec3f(0));
                    plane.SetNormal(Vec3f(0, 0, 1));

                    SceneObjectParams p;

                    camera->CastRay(GetCursorPos(), plane, p.pos);

                    GetGeometryManager().CreateSceneObject("lambda_test", p);
                }));

            GetEventSystem().SetProcessor(
                "LB", AppEventProcessor([this]() // left mouse button pressed
            {
                // choose object here
                selectedObject = nullptr;
                Figure* f =
                    GetDebugRenderer().FindFigure(*camera, GetCursorPos());
                if (f != nullptr) {
                    selectedObject = f->GetSceneObject();
                }
            },
                                         [this]() // left mouse button is being
                                                  // pressed
            {
                          // move object if choosen
                          if (selectedObject != nullptr) {
                              Plane plane;
                              plane.SetPoint(selectedObject->GetPos());
                              plane.SetNormal(Vec3f(0, 0, 1));

                              Vec3f pos;

                              camera->CastRay(GetCursorPos(), plane, pos);

                              selectedObject->SetPos(pos);
                          }
                      }));

            GetEventSystem().SetProcessor(
                "LB C-d", AppEventProcessor([this]() // control-d pressed after
                                                      // LB released
            {
                // delete object if choosen
                if (selectedObject != nullptr) {
                    GetScene().DestroyObject(selectedObject);
                    selectedObject = nullptr;
                }
            }));

            GetEventSystem().SetProcessor(
                "C-x C-c",
                AppEventProcessor([this]() // key pressed. emacs like :)
            { this->Quit(); }));
        }

    } // namespace test

} // namespace drash
