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

        CTest3::CTest3() {
            SetProcessors();

            InitObjects();

            auto& c = GetCamera();
            c.GetPos().SetTarget({ 0, 50, 180 }, 1.0f,
                                 AnimatorBehavior::Single);
            c.GetRotation().Set(CVec3f(-M_PI / 12.0f, 0.0f, 0.0f));

            slider1.Connect(&GetUISystem());
            slider1.SetPos(CVec2i(10, 10));
            slider1.SetSize(CVec2ui(100, 20));
            slider1.SetMin(-50);
            slider1.SetMax(50);
            slider1.SetValue(0);
            slider1.SetValueHandler([this](float _value) {
                GetScene().SetGravity(CVec2f(_value, slider2.GetValue()));
            });

            slider2.Connect(&GetUISystem());
            slider2.SetPos(CVec2i(10, 40));
            slider2.SetSize(CVec2ui(100, 20));
            slider2.SetMin(-50);
            slider2.SetMax(50);
            slider2.SetValue(-9.8);
            slider2.SetValueHandler([this](float _value) {
                GetScene().SetGravity(CVec2f(slider1.GetValue(), _value));
            });

            light1.position.Set(0, 40, 0);

            GetDebugRenderer().SetLight(&light1);
        }

        void CTest3::Step(double _dt) { CTest1::Step(_dt); }

        float angle = 0;

        void CTest3::Render() {
            CTest1::Render();

            CVec4f origin(40, 20, 0, 1);
            CVec4f pos(0, 0, 0, 1);
            CMatrix4f m;
            MatrixRotationX(m, angle);
            MatrixMultiply(m, origin, pos);
            GetGrengSystems().GetRenderer().DrawPoint(GetCamera(), pos, 10,
                                                      CColor4f(1, 0, 0), false);

            angle += 0.01;
        }

        void CTest3::SetProcessors() {
            GetEventSystem().SetMode("editor_mode");

            GetEventSystem().SetProcessor("C-x",
                                          CAppEventProcessor([]() {}, [this]() {
                                              GetEventSystem().SetMode("test3");
                                          }));

            GetEventSystem().SetMode("test3");

            GetEventSystem().SetProcessor(
                "C-x", CAppEventProcessor([]() {}, [this]() {
                           GetEventSystem().SetMode("editor_mode");
                       }));

            GetEventSystem().SetProcessor(
                "w", CAppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Deep);
                     }));

            GetEventSystem().SetProcessor(
                "a", CAppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Left);
                     }));

            GetEventSystem().SetProcessor(
                "s", CAppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::AntiDeep);
                     }));

            GetEventSystem().SetProcessor(
                "d", CAppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Right);
                     }));

            GetEventSystem().SetProcessor(
                "SPC", CAppEventProcessor([]() {}, [this]() {
                           this->GetPlayersSystem().SendMessage(
                               GetPlayersSystem().GetPlayers()[0],
                               PlayerMessage::Jump);
                       }));

            GetEventSystem().SetProcessor(
                "MB",
                CAppEventProcessor([this]() {
                                       if (moveObject == nullptr) {
                                           CFigure* f =
                                               GetDebugRenderer().FindFigure(
                                                   GetCamera(), GetCursorPos());
                                           if (f != nullptr) {
                                               moveObject = f->GetSceneObject();
                                           }
                                       }
                                   },
                                   [this]() {
                                       if (moveObject != nullptr) {
                                           CPlane p(PlaneXY);
                                           p.SetPoint(moveObject->GetPos());

                                           CVec3f pos;

                                           GetCamera().CastRay(GetCursorPos(),
                                                               p, pos);

                                           pos.Vec2() -=
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
                            moveObject->SetLinearVelocity(CVec2f(0));
                        }
                        moveObject = nullptr;
                    }
                }));

            GetEventSystem().SetProcessor(
                "WHUP", CAppEventProcessor([this]() {
                            CVec3f pos;

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
                "WHDN", CAppEventProcessor([this]() {
                            CVec3f pos;

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
                "C-q", CAppEventProcessor([this]() { this->Quit(); }));

            GetEventSystem().SetProcessor(
                "e", CAppEventProcessor([this]() {},
                                        [this]() { GetCamera().Forward(5); }));

            GetEventSystem().SetProcessor(
                "q", CAppEventProcessor([this]() {},
                                        [this]() { GetCamera().Forward(-5); }));

            GetEventSystem().SetProcessor(
                "z", CAppEventProcessor([this]() {},
                                        [this]() { GetCamera().Strafe(5); }));

            GetEventSystem().SetProcessor(
                "c", CAppEventProcessor([this]() {},
                                        [this]() { GetCamera().Strafe(-5); }));
        }

        void CTest3::InitObjects() {
            CSceneObjectGeometry sbg;
            sbg.figures.resize(1);
            sbg.figures[0].depth = 200;
            sbg.figures[0].restitution = 0.0;
            sbg.figures[0].vertices.push_back(CVec2f(-300, -5));
            sbg.figures[0].vertices.push_back(CVec2f(300, -5));
            sbg.figures[0].vertices.push_back(CVec2f(300, 5));
            sbg.figures[0].vertices.push_back(CVec2f(-300, 5));
            CSceneObjectParams sbp;
            sbp.dynamic = false;
            sbp.angle = 0;
            sbp.pos.Set(0, -30, 0);
            GetScene().CreateObject(sbg, sbp);

            sbp.pos.Set(0, 570, 0);
            GetScene().CreateObject(sbg, sbp);

            sbg.figures[0].vertices.clear();
            sbg.figures.resize(1);
            sbg.figures[0].vertices.push_back(CVec2f(-5, -300));
            sbg.figures[0].vertices.push_back(CVec2f(5, -300));
            sbg.figures[0].vertices.push_back(CVec2f(5, 300));
            sbg.figures[0].vertices.push_back(CVec2f(-5, 300));
            sbp.pos.Set(-300, 270, 0);
            GetScene().CreateObject(sbg, sbp);

            sbg.figures[0].vertices.clear();
            sbg.figures.resize(1);
            sbg.figures[0].vertices.push_back(CVec2f(-5, -300));
            sbg.figures[0].vertices.push_back(CVec2f(5, -300));
            sbg.figures[0].vertices.push_back(CVec2f(5, 300));
            sbg.figures[0].vertices.push_back(CVec2f(-5, 300));
            sbp.pos.Set(300, 270, 0);
            GetScene().CreateObject(sbg, sbp);

            CSceneObjectGeometry pg;
            pg.figures.resize(1);
            pg.figures[0].mass = 3;
            CSceneObjectParams pp;
            pp.pos.Set(-200, 100, 0);
            GetScene().CreateObject(pg, pp)->SetLinearVelocity(CVec2f(200, 0));

            CSceneObjectGeometry ppg;
            ppg.figures.resize(1);
            ppg.figures[0].depth = 2;
            CPlayerParams ppp;
            ppp.sceneObjectParams.pos.Set(0, 10, 0);
            ppp.velocityLimit = 10;
            GetPlayersSystem().CreatePlayer(ppg, ppp);
        }

    } // namespace test

} // namespace drash
