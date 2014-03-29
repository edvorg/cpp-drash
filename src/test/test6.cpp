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

#include "test6.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"
#include "../players/player.h"
#include "../greng/camera.h"
#include "../scene/scene.h"
#include "../levelmanager/leveldesc.h"
#include "../misc/math.h"

namespace drash {

    namespace test {

        CTest6::CTest6() : CTest1() {
            InitCamera();
            InitLevel();
            InitPlayer();
            InitLight();
            InitProcessors();
            InitTarget();

            GetDebugRenderer().SetTexCoordsScale(0.5);
        }

        void CTest6::Step(double _dt) {
            CTest1::Step(_dt);

            targetCreateTimer += _dt;
            angle += _dt;

            while (angle >= M_PI * 2.0) {
                angle -= M_PI * 2.0;
            }

            light1.position.Set(sin(angle) * 40, 20, 0);

            GetDebugRenderer().SetLight(&light1);

            CVec2f dir(GetPlayersSystem()
                               .GetPlayers()[0]
                               ->GetSceneObject()
                               ->GetPosXY()
                               .Get()
                               .x -
                           player1OldPos.x,
                       player1OldPos.z - GetPlayersSystem()
                                             .GetPlayers()[0]
                                             ->GetSceneObject()
                                             ->GetPosZ());

            if (dir.LengthSquared() < 0.00001) {
                dir.Set(1, 0);
            } else {
                dir.Normalize();
            }

            float angle =
                acos(dir.x * player1MeshDir.x + dir.y * player1MeshDir.y);

            if (angle > 0.000001f) {
                CVec3f cross;
                Vec3Cross(CVec3f(player1MeshDir, 0), CVec3f(dir, 0), cross);

                CMatrix4f m;
                if (cross.z < 0.0) {
                    MatrixRotationZ(m, angle * -10.0 * _dt);
                } else {
                    MatrixRotationZ(m, angle * 10.0 * _dt);
                }

                CVec4f new_dir;
                MatrixMultiply(m, CVec4f(player1MeshDir, 0, 0), new_dir);
                player1MeshDir = new_dir;

                if (player1MeshDir.LengthSquared() > 0.00001f) {
                    player1MeshDir.Normalize();
                }
            }

            player1OldPos = player1->GetSceneObject()->GetPos();

            if (followPlayer == true) {
                CVec3f t = GetCamera().GetPos().GetTarget();
                t.x = player1->GetSceneObject()->GetPos().x;
                GetCamera().GetPos().SetTarget(t, 1.0,
                                               AnimatorBehavior::Single);
            }

            if (targetCreateTimer > 5.0 && targetDestroyed) {
                CSceneObjectParams p;
                p.pos.Set(math::Rand<float>(-5, 5, 0.5), 10, 0);

                auto o = GetScene().CreateObject(*targetGeometry, p);

                targetDestroyed = false;
                targetCreateTimer = 0.0;

                o->AddDestroyHandler([this](CSceneObject*) {
                    targetDestroyed = true;
                });
            }
        }

        void CTest6::Render() {
            CTest1::Render();

            float new_angle = acos(player1MeshDir.x);

            if (player1MeshDir.y < 0.0f) {
                new_angle *= -1;
            }

            CMatrix4f r;
            MatrixRotationY(r, M_PI * 0.5 + new_angle);

            CMatrix4f s;
            MatrixScale(s, CVec3f(0.4));

            CMatrix4f rot;
            MatrixMultiply(r, s, rot);

            CMatrix4f transl;
            MatrixTranslation(transl, player1->GetSceneObject()->GetPos() -
                                          CVec3f(0, 0.4, 0));

            CMatrix4f model;
            MatrixMultiply(transl, rot, model);

            CMatrix4f model_view;
            MatrixMultiply(GetCamera().GetViewMatrix(), model, model_view);

            for (unsigned int i = 0; i < 6; i++) {
                GetGrengSystems().GetRenderer().RenderMesh(
                    player1Mesh, i, &player1Texture, 1, player1ShaderProgram,
                    &model, nullptr, &model_view,
                    &GetCamera().GetProjectionMatrix(), &light1);
            }

            GetGrengSystems().GetRenderer().DrawPoint(
                GetCamera(), light1.position, 10, CColor4f(1, 1, 1, 1), true);
        }

        bool CTest6::InitCamera() {
            GetCamera().GetPos().Set(CVec3f(0, 15, 30));
            GetCamera().GetRotation().Set(CVec2f(-M_PI / 12.0, 0));

            return true;
        }

        bool CTest6::InitLevel() {
            std::map<std::string, CSceneObjectGeometry*>& geometries =
                GetGeometryManager().GetGeometries();

            for (auto i = geometries.begin(); i != geometries.end(); i++) {
                i->second->ComputeDestructionGraph(0.5);
            }

            CLevelDesc* l = GetLevelManager().CreateLevel();

            if (l == nullptr) {
                return false;
            }

            l->Load("demo_level.dlvl");

            if (GetLevelManager().StartLevel(l) == false) {
                return false;
            }

            auto i = GetGeometryManager().GetGeometries().find("Object3");

            if (i != GetGeometryManager().GetGeometries().end()) {
                CSceneObjectParams p;
                p.pos.Set(3, -2, 0);
                p.dynamic = false;

                auto o = GetScene().CreateObject(*i->second, p);

                if (o == nullptr) {
                    return false;
                }

                o->AddContactBeginHandler([this](CFigure* _f1, CFigure* _f2) {
                    if (_f2->GetSceneObject() == player1->GetSceneObject()) {
                        _f1->GetSceneObject()->GetPosXY().SetTarget(
                            CVec2f(8, 8), 2.0, AnimatorBehavior::Single);
                    }
                });

                o->AddContactEndHandler([this](CFigure* _f1, CFigure* _f2) {
                    if (_f2->GetSceneObject() == player1->GetSceneObject()) {
                        _f1->GetSceneObject()->GetPosXY().SetTarget(
                            CVec2f(3, -2), 2.0, AnimatorBehavior::Single);
                    }
                });
            }

            return true;
        }

        bool CTest6::InitPlayer() {
            CSceneObjectGeometry g1;

            g1.figures.resize(1);
            g1.figures[0].depth = 0.8;
            g1.figures[0].vertices.push_back(CVec2f(0, 0.4));
            g1.figures[0].vertices.push_back(CVec2f(-0.4, -0.2));
            g1.figures[0].vertices.push_back(CVec2f(0, -0.4));
            g1.figures[0].vertices.push_back(CVec2f(0.4, -0.2));

            CPlayerParams p1;
            p1.velocityLimit = 10;
            p1.sceneObjectParams.fixedRotation = true;
            p1.sceneObjectParams.pos.Set(0, 20, 0);

            player1 = GetPlayersSystem().CreatePlayer(g1, p1);

            if (player1 == nullptr) {
                return false;
            }

            player1Mesh = GetGrengSystems().GetMeshManager().CreateMeshCube();

            if (player1Mesh == nullptr) {
                return false;
            }

            GetGrengSystems().GetMeshManager().ComputeNormals(player1Mesh);

            player1Texture =
                GetGrengSystems().GetTextureManager().CreateTextureDummy();

            if (player1Texture == nullptr) {
                return false;
            }

            player1VertexShader =
                GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile(
                    "shaders/shader2.120.vs");

            if (player1VertexShader == nullptr) {
                return false;
            }

            player1FragmentShader =
                GetGrengSystems()
                    .GetFragmentShaderManager()
                    .CreateShaderFromFile("shaders/shader2.120.fs");

            if (player1FragmentShader == nullptr) {
                return false;
            }

            player1ShaderProgram =
                GetGrengSystems().GetShaderProgramManager().CreateProgram(
                    player1VertexShader, player1FragmentShader);

            if (player1ShaderProgram == nullptr) {
                return false;
            }

            return true;
        }

        bool CTest6::InitLight() {
            light1.position.Set(10, 10, 0);

            GetDebugRenderer().SetLight(&light1);

            return true;
        }

        bool CTest6::InitProcessors() {
            GetEventSystem().SetMode("editor_mode");

            GetEventSystem().SetProcessor("C-x",
                                          CAppEventProcessor([]() {}, [this]() {
                                              followPlayer = true;
                                              GetEventSystem().SetMode("test6");
                                          }));

            GetEventSystem().SetMode("test6");

            GetEventSystem().SetProcessor(
                "C-x", CAppEventProcessor([]() {}, [this]() {
                           followPlayer = false;
                           GetEventSystem().SetMode("editor_mode");
                       }));

            GetEventSystem().SetProcessor(
                "C-q", CAppEventProcessor([this]() { this->Quit(); }));

            GetEventSystem().SetProcessor(
                "w",
                CAppEventProcessor([]() {}, [this]() {
                    //        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0],
                    // PlayerMessage::Deep);
                }));

            GetEventSystem().SetProcessor(
                "a", CAppEventProcessor([]() {}, [this]() {
                         this->GetPlayersSystem().SendMessage(
                             GetPlayersSystem().GetPlayers()[0],
                             PlayerMessage::Left);
                     }));

            GetEventSystem().SetProcessor(
                "s",
                CAppEventProcessor([]() {}, [this]() {
                    //        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0],
                    // PlayerMessage::AntiDeep);
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
                "LB", CAppEventProcessor([this]() {
                          CSceneObjectGeometry g;
                          g.figures.resize(1);
                          g.figures[0].vertices.push_back(CVec2f(-0.1, -0.1));
                          g.figures[0].vertices.push_back(CVec2f(0.1, -0.1));
                          g.figures[0].vertices.push_back(CVec2f(0.1, 0.1));
                          g.figures[0].vertices.push_back(CVec2f(-0.1, 0.1));
                          g.figures[0].depth = 0.2;

                          CPlane pl(PlaneXY);
                          CVec3f pos;
                          GetCamera().CastRay(GetCursorPos(), pl, pos);
                          pos.Vec2() -=
                              player1->GetSceneObject()->GetPos().Vec2();
                          pos *= 2;

                          CSceneObjectParams p;
                          p.pos = player1->GetSceneObject()->GetPos();

                          if (pos.x > 0.0f) {
                              p.pos.x += 1;
                          } else {
                              p.pos.x -= 1;
                          }

                          CSceneObject* o = GetScene().CreateObject(g, p);
                          o->SetLinearVelocity(pos.Vec2());
                      }));

            return true;
        }

        bool CTest6::InitTarget() {
            auto i = GetGeometryManager().GetGeometries().find("Object1");

            if (i == GetGeometryManager().GetGeometries().end()) {
                return false;
            }

            targetGeometry = i->second;

            if (targetGeometry == nullptr) {
                return false;
            }

            return true;
        }

    } // namespace test
} // namespace drash
