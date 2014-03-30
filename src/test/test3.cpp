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

#include "test3.h"

#include "../scene/scene.h"
#include "../players/player.h"
#include "../scene/figure.h"
#include "../greng/camera.h"

namespace drash {

    namespace test {

        Test3::Test3(greng::Greng& greng) : Test1(greng) {
            SetProcessors();

            InitObjects();

            auto& c = GetCamera();
            c.GetPos().SetTarget({ 0, 50, 180 }, 1.0f,
                                 AnimatorBehavior::Single);
            c.GetRotation().Set(Vec3f(-M_PI / 12.0f, 0.0f, 0.0f));

            slider1.Connect(&GetUISystem());
            slider1.SetPos(Vec2i(10, 10));
            slider1.SetSize(Vec2ui(100, 20));
            slider1.SetMin(-50);
            slider1.SetMax(50);
            slider1.SetValue(0);
            slider1.SetValueHandler([this](float _value) {
                GetScene().SetGravity(Vec2f(_value, slider2.GetValue()));
            });

            slider2.Connect(&GetUISystem());
            slider2.SetPos(Vec2i(10, 40));
            slider2.SetSize(Vec2ui(100, 20));
            slider2.SetMin(-50);
            slider2.SetMax(50);
            slider2.SetValue(-9.8);
            slider2.SetValueHandler([this](float _value) {
                GetScene().SetGravity(Vec2f(slider1.GetValue(), _value));
            });

            light1.position.Set(0, 40, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void Test3::Step(double _dt) { Test1::Step(_dt); }

        float angle = 0;

        void Test3::Render() {
            Test1::Render();

            Vec4f origin(40, 20, 0, 1);
            Vec4f pos(0, 0, 0, 1);
            Matrix4f m;
            MatrixRotationX(m, angle);
            MatrixMultiply(m, origin, pos);
            GetGreng().GetRenderer().DrawPoint(GetCamera(), pos, 10,
                                                      Color4f(1, 0, 0), false);

            angle += 0.01;
        }

        void Test3::SetProcessors() {
            GetEventSystem().SetMode("editor_mode");

            GetEventSystem().SetProcessor("C-x",
                                          AppEventProcessor([]() {}, [this]() {
                                              GetEventSystem().SetMode("test3");
                                          }));

            GetEventSystem().SetMode("test3");

            GetEventSystem().SetProcessor(
                "C-x", AppEventProcessor([]() {}, [this]() {
                           GetEventSystem().SetMode("editor_mode");
                       }));

            GetEventSystem().SetProcessor(
                "w", AppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Deep);
                     }));

            GetEventSystem().SetProcessor(
                "a", AppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Left);
                     }));

            GetEventSystem().SetProcessor(
                "s", AppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::AntiDeep);
                     }));

            GetEventSystem().SetProcessor(
                "d", AppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Right);
                     }));

            GetEventSystem().SetProcessor(
                "SPC", AppEventProcessor([]() {}, [this]() {
                           this->GetPlayersSystem().SendMessage(
                               GetPlayersSystem().GetPlayers()[0],
                               PlayerMessage::Jump);
                       }));

            GetEventSystem().SetProcessor(
                "MB",
                AppEventProcessor([this]() {
                                       if (moveObject == nullptr) {
                                           Figure* f =
                                               GetDebugRenderer().FindFigure(
                                                   GetCamera(), GetCursorPos());
                                           if (f != nullptr) {
                                               moveObject = f->GetSceneObject();
                                           }
                                       }
                                   },
                                   [this]() {
                                       if (moveObject != nullptr) {
                                           Plane p(PlaneXY);
                                           p.SetPoint(moveObject->GetPos());

                                           Vec3f pos;

                                           GetCamera().CastRay(GetCursorPos(),
                                                               p, pos);

                                           pos.AsVec2() -=
                                               moveObject->GetMassCenter();
                                           pos *= 10;
                                           moveObject->SetLinearVelocity(pos);
                                       }
                                   },
                                   [this]() {
                    if (moveObject != nullptr) {
                        /// if our body is not dynamic. it wil never stop, until
                        /// we make it's velocity module to 0
                        if (moveObject->IsDynamic() == false) {
                            moveObject->SetLinearVelocity(Vec2f(0));
                        }
                        moveObject = nullptr;
                    }
                }));

            GetEventSystem().SetProcessor(
                "WHUP", AppEventProcessor([this]() {
                            Vec3f pos;

                            if (GetCamera().GetPos().IsTargetSet()) {
                                pos = GetCamera().GetPos().GetTarget();
                            } else {
                                pos = GetCamera().GetPos().Get();
                            }

                            pos.z += 10.0f;

                            GetCamera().GetPos().SetTarget(
                                pos, 0.3, AnimatorBehavior::Single);
                        }));

            GetEventSystem().SetProcessor(
                "WHDN", AppEventProcessor([this]() {
                            Vec3f pos;

                            if (GetCamera().GetPos().IsTargetSet()) {
                                pos = GetCamera().GetPos().GetTarget();
                            } else {
                                pos = GetCamera().GetPos().Get();
                            }

                            pos.z -= 10.0f;

                            GetCamera().GetPos().SetTarget(
                                pos, 0.3, AnimatorBehavior::Single);
                        }));

            GetEventSystem().SetProcessor(
                "C-q", AppEventProcessor([this]() { this->Quit(); }));

            GetEventSystem().SetProcessor(
                "e", AppEventProcessor([this]() {},
                                        [this]() { GetCamera().Forward(5); }));

            GetEventSystem().SetProcessor(
                "q", AppEventProcessor([this]() {},
                                        [this]() { GetCamera().Forward(-5); }));

            GetEventSystem().SetProcessor(
                "z", AppEventProcessor([this]() {},
                                        [this]() { GetCamera().Strafe(5); }));

            GetEventSystem().SetProcessor(
                "c", AppEventProcessor([this]() {},
                                        [this]() { GetCamera().Strafe(-5); }));
        }

        void Test3::InitObjects() {
            SceneObjectGeometry sbg;
            sbg.figures.resize(1);
            sbg.figures[0].depth = 200;
            sbg.figures[0].restitution = 0.0;
            sbg.figures[0].vertices.push_back(Vec2f(-300, -5));
            sbg.figures[0].vertices.push_back(Vec2f(300, -5));
            sbg.figures[0].vertices.push_back(Vec2f(300, 5));
            sbg.figures[0].vertices.push_back(Vec2f(-300, 5));
            SceneObjectParams sbp;
            sbp.dynamic = false;
            sbp.angle = 0;
            sbp.pos.Set(0, -30, 0);
            GetScene().CreateObject(sbg, sbp);

            sbp.pos.Set(0, 570, 0);
            GetScene().CreateObject(sbg, sbp);

            sbg.figures[0].vertices.clear();
            sbg.figures.resize(1);
            sbg.figures[0].vertices.push_back(Vec2f(-5, -300));
            sbg.figures[0].vertices.push_back(Vec2f(5, -300));
            sbg.figures[0].vertices.push_back(Vec2f(5, 300));
            sbg.figures[0].vertices.push_back(Vec2f(-5, 300));
            sbp.pos.Set(-300, 270, 0);
            GetScene().CreateObject(sbg, sbp);

            sbg.figures[0].vertices.clear();
            sbg.figures.resize(1);
            sbg.figures[0].vertices.push_back(Vec2f(-5, -300));
            sbg.figures[0].vertices.push_back(Vec2f(5, -300));
            sbg.figures[0].vertices.push_back(Vec2f(5, 300));
            sbg.figures[0].vertices.push_back(Vec2f(-5, 300));
            sbp.pos.Set(300, 270, 0);
            GetScene().CreateObject(sbg, sbp);

            SceneObjectGeometry pg;
            pg.figures.resize(1);
            pg.figures[0].mass = 3;
            SceneObjectParams pp;
            pp.pos.Set(-200, 100, 0);
            GetScene().CreateObject(pg, pp)->SetLinearVelocity(Vec2f(200, 0));

            SceneObjectGeometry ppg;
            ppg.figures.resize(1);
            ppg.figures[0].depth = 2;
            PlayerParams ppp;
            ppp.sceneObjectParams.pos.Set(0, 10, 0);
            ppp.velocityLimit = 10;
            GetPlayersSystem().CreatePlayer(ppg, ppp);
        }

    } // namespace test

} // namespace drash
