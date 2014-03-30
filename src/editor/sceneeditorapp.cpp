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

#include "sceneeditorapp.h"
#include "../scene/sceneobject.h"
#include "../levelmanager/leveldesc.h"
#include "../diag/logger.h"
#include "../misc/plane.h"

#include <QDebug>

using namespace greng;
using namespace std;

namespace drash {

    SceneEditorApp::SceneEditorApp() {}

    SceneEditorApp::SceneEditorApp() {

        if (App::App() == false) {
            return false;
        }

        if (InitCamera() == false || InitPointLight() == false ||
            InitSpecialPoint() == false) {
            return false;
        }

        SetProcessors();
        SetCameraProcessors();
        SetDragDropProcessors();
        GetDebugRenderer().SetTexCoordsScale(0.5);
        timer.Reset(true);

        //    moveablePoint.SetSize(100);
        return true;
        // GetGeometryManager().CreateSceneObjectFromTemplate("Object1",SceneObjectParams());
    }

    void SceneEditorApp::Step(double _dt) {
        timer.Tick();
        if (playLevel == true) {
            App::Step(_dt);
        } else {
            App::Step(0);
        }
        if (selectedObject != nullptr) {
            moveablePoint.SetCursorPos(GetCursorPos());
            rotationablePoint.SetCursorPos(GetCursorPos());
            moveablePoint.Step(_dt);
            rotationablePoint.Step(_dt);

            MoveOfAxis();
            RatateObject();
        }
    }

    void SceneEditorApp::Render() {
        if (playLevel == true || paused == true) {
            App::Render();
        } else {
            if (currentLevel != nullptr) {
                for (unsigned int i = 0; i < currentLevel->EnumObjects(); i++) {
                    auto g = GetGeometryManager().GetGeometry(
                        currentLevel->GetObjects()[i]->geometryName);

                    if (g != nullptr) {
                        GetDebugRenderer().RenderObject(
                            *g, currentLevel->GetObjects()[i]->params);
                    }
                }
            }

            RenderDragTemplate();
            RenderPoints();
        }
    }

    void SceneEditorApp::UpdateTemplateSystem() {
        GetGeometryManager().Load();
    }

    bool SceneEditorApp::LoadLevel(const std::string& _filename) {
        if (currentLevel != nullptr) {
            GetLevelManager().DestroyLevel(currentLevel);
            currentLevel = nullptr;
        }

        selectedObject = nullptr;
        currentLevel = GetLevelManager().CreateLevel();

        if (currentLevel == nullptr) {
            return false;
        }

        if (currentLevel->Load(_filename) == false) {
            return false;
        }

        fileNameLevel = _filename;

        treeRefreshHandler();
        GetLevelManager().StartLevel(currentLevel);
        playLevel = false;
        return true;
        // GetLevelManager().StartLevel(level);
    }

    bool SceneEditorApp::SaveLevelAs(const std::string& _filename) {
        if (currentLevel == nullptr) {
            LOG_ERR("Level not saved, because not created.");
            return false;
        }

        bool result = currentLevel->Store(_filename);

        if (result == true) {
            fileNameLevel = _filename;
        }

        return result;
    }

    bool SceneEditorApp::NewLevel() {
        selectedObject = nullptr;
        if (currentLevel != nullptr) {
            GetLevelManager().DestroyLevel(currentLevel);
        }
        currentLevel = GetLevelManager().CreateLevel();
        if (currentLevel == nullptr) {
            return false;
        }
        GetLevelManager().StartLevel(currentLevel);
        fileNameLevel = "";
        return true;
    }

    void SceneEditorApp::StartCurrentLevel() {
        if (currentLevel != nullptr) {
            if (paused == false) {
                GetLevelManager().StartLevel(currentLevel);
            }

            playLevel = true;
            selectedObject = nullptr;
        }
    }

    void SceneEditorApp::AddObject(const std::string& _name,
                                    const Vec3f& _pos) {
        if (currentLevel == nullptr) {
            LOG_WARN("Not set current level");
            return;
        }

        SceneObjectGeometry* obj = GetGeometryManager().GetGeometry(_name);
        if (obj == nullptr) {
            LOG_ERR(_name.c_str() << " not found in Template System");
            return;
        }

        std::string obj_name = currentLevel->GetUniqueObjectName();

        LevelObjectDesc* p = currentLevel->AddObject(_name, obj_name);
        if (p == nullptr) {
            return;
        }

        p->params.pos = _pos;
        p->geometryName = _name;
        p->levelObjectName = obj_name;

        treeRefreshHandler();
    }

    void SceneEditorApp::SetProcessors() {
        GetEventSystem().SetProcessor(
            "RB", AppEventProcessor([this]() {
                      if (playLevel == true) {
                          return;
                      }
                      selectedObject = SelectObject();
                      if (selectedObject != nullptr) {
                          oldpositon = selectedObject->params.pos;
                          moveablePoint.SetCenter(oldpositon);
                          rotationablePoint.SetPoint(oldpositon);
                          rotationablePoint.SetRotation(
                              Vec3f(0, 0, selectedObject->params.angle));
                      }
                  }));

        GetEventSystem().SetProcessor(
            "LB", AppEventProcessor([this]() {
                                         if (playLevel == true) {
                                             return;
                                         }
                                         if (selectedObject != nullptr) {
                                             moveablePoint.ClickBegin();
                                             rotationablePoint.RotateBegin();
                                         }
                                     },
                                     [this]() {
                                         if (playLevel == true) {
                                             return;
                                         }
                                         if (selectedObject != nullptr) {
                                             moveablePoint.ClickPressing();
                                         }
                                     },
                                     [this]() {
                      if (playLevel == true) {
                          return;
                      }
                      if (selectedObject != nullptr) {
                          moveablePoint.ClickEnd();
                          rotationablePoint.RotateEnd();
                      }
                  }));

        GetEventSystem().SetProcessor("C-LB", AppEventProcessor([this]() {

                                                  if (playLevel) {
                                                      return;
                                                  }
                                                  LevelObjectDesc* temp =
                                                      SelectObject();
                                                  if (temp != nullptr) {
                                                      temp->params.dynamic =
                                                          !temp->params.dynamic;
                                                  }
                                              }));
    }

    void SceneEditorApp::SetCameraProcessors() {
        GetEventSystem().SetProcessor(
            "MB",
            AppEventProcessor([this]() { camRotFirstClick = GetCursorPos(); },
                               [this]() {
                Vec2f new_pos = GetCursorPos();

                Vec2f rot = camera->GetRotation().Get();
                rot.y -= new_pos.x - camRotFirstClick.x;
                rot.x += new_pos.y - camRotFirstClick.y;

                camera->GetRotation().Set(rot);

                camRotFirstClick = new_pos;
            }));

        GetEventSystem().SetProcessor(
            "w", AppEventProcessor([this]() {},
                                    [this]() {
                                        camera->Forward(MOVING_SPEED *
                                                        timer.GetDeltaTime());
                                    },
                                    [this] {}));

        GetEventSystem().SetProcessor(
            "a", AppEventProcessor([this]() {}, [this]() {
                     camera->Strafe(MOVING_SPEED * timer.GetDeltaTime());
                 }));

        GetEventSystem().SetProcessor(
            "s", AppEventProcessor([this]() {}, [this]() {
                     camera->Forward(-MOVING_SPEED * timer.GetDeltaTime());
                 }));

        GetEventSystem().SetProcessor(
            "d", AppEventProcessor([this]() {}, [this]() {
                     camera->Strafe(-MOVING_SPEED * timer.GetDeltaTime());
                 }));
    }

    void SceneEditorApp::SetDragDropProcessors() {
        GetEventSystem().SetProcessor("DRL", AppEventProcessor([this]() {
                                                 dragNow = false;
                                                 // dragTemplateName = "";
                                             }));

        GetEventSystem().SetProcessor(
            "DRDP", AppEventProcessor([this]() {
                                           if (currentLevel != nullptr) {
                                               // qDebug() << "Proccess enter";
                                               dragTemplateName =
                                                   getSelectedTemplateHandler();
                                               dragNow = true;
                                           }
                                       },
                                       [this]() {}, [this]() {

                        if (dragNow == true) {
                            Plane plane;
                            plane.SetNormal(Vec3f(0, 0, 1));
                            plane.SetPoint(Vec3f(0, 0, 0));

                            Vec3f position;
                            Vec2f cpos = GetCursorPos();
                            camera->CastRay(cpos, plane, position);
                            AddObject(dragTemplateName, position);

                            dragNow = false;
                            dragTemplateName = "";
                        }
                    }));
    }

    bool SceneEditorApp::InitCamera() {
        greng::CameraParams p;
        p.pos.Set(10, 10, 10.0f);
        p.rotation.Set(-M_PI / 4, M_PI / 4, 0);
        camera = GetGreng().GetCameraManager().CreateCamera(p);

        if (camera == nullptr) {
            return false;
        }

        GetDebugRenderer().SetCamera(camera);

        return true;
    }

    bool SceneEditorApp::InitPointLight() {
        light1.position.Set(0, 10, 0);
        GetDebugRenderer().SetLight(&light1);
        return true;
    }

    bool SceneEditorApp::InitSpecialPoint() {
        moveablePoint.SetCamera(camera);

        rotationablePoint.SetRenderer(&GetGreng().GetRenderer());

        rotationablePoint.SetCamera(camera);

        if (rotationablePoint.Init() == false) {
            return false;
        }

        rotationablePoint.SetAxisOX(false);
        rotationablePoint.SetAxisOY(false);

        return true;
    }

    void SceneEditorApp::RatateObject() {
        if (selectedObject == nullptr) {
            return;
        }

        float anglez = rotationablePoint.GetRotation().z;
        selectedObject->params.angle = anglez;
        changedParams = true;
    }

    void SceneEditorApp::RenderPoints() {
        if (selectedObject == nullptr) {
            return;
        }

        moveablePoint.Render(GetGreng().GetRenderer());
        rotationablePoint.Render();
    }

    void SceneEditorApp::RenderDragTemplate() {
        if (currentLevel == nullptr) {
            return;
        }
        if (dragNow == false) {
            return;
        }
        // qDebug() << dragTemplateName.c_str();
        if (dragTemplateName != "") {
            SceneObjectGeometry* g =
                GetGeometryManager().GetGeometry(dragTemplateName);
            if (g == nullptr) {
                return;
            }

            SceneObjectParams params;
            Vec3f position;
            Vec2f cpos = GetCursorPos();

            Plane plane;
            plane.SetNormal(Vec3f(0, 0, 1));
            plane.SetPoint(Vec3f(0, 0, 0));

            //        LOG_INFO(cpos);
            camera->CastRay(cpos, plane, position);
            //        v = vv;
            params.pos = position;
            //        LOG_INFO(position);
            GetDebugRenderer().RenderObject(*g, params);
        }
    }

    LevelObjectDesc* SceneEditorApp::SelectObject() {
        auto f = [this](unsigned int i) {
            return currentLevel->GetObjects()[i];
        };

        return GetDebugRenderer().FindObject(camera, GetCursorPos(), f,
                                             currentLevel->EnumObjects());
    }

    void SceneEditorApp::MoveOfAxis() {
        selectedObject->params.pos = moveablePoint.GetCenter();
        oldpositon = moveablePoint.GetCenter();
        rotationablePoint.SetPoint(moveablePoint.GetCenter());
    }

    void SceneEditorApp::StopLevel() {
        playLevel = false;
        ResetLevel();
        paused = false;
        //    GetLevelManager().StartLevel(currentLevel,objectParams);
    }

    void SceneEditorApp::ResetLevel() {
        if (currentLevel != nullptr) {
            GetLevelManager().StartLevel(currentLevel);
            paused = false;
        }
    }

    void SceneEditorApp::LookObject(const std::string& _geometryname,
                                     const std::string& _objectname) {
        if (currentLevel != nullptr) {
            for (unsigned int i = 0; i < currentLevel->EnumObjects(); i++) {
                if (currentLevel->GetObjects()[i]->levelObjectName ==
                        _objectname &&
                    currentLevel->GetObjects()[i]->geometryName ==
                        _geometryname) {
                    camera->LookAt(currentLevel->GetObjects()[i]->params.pos);
                }
            }
        }
    }

    void SceneEditorApp::SetDynamicParam(bool _val) {
        if (selectedObject != nullptr) {
            selectedObject->params.dynamic = _val;
        }
    }

    void SceneEditorApp::SetFixedRotationParam(bool _val) {
        if (selectedObject != nullptr) {
            selectedObject->params.fixedRotation = _val;
        }
    }

    void SceneEditorApp::SetAngleParams(float _angle) {
        if (selectedObject != nullptr) {
            selectedObject->params.angle = _angle;
        }
    }

    float SceneEditorApp::GetAngleParams() const {
        if (IsObjectSelected() == true) {
            return selectedObject->params.angle;
        }
        return 0.0f;
    }

    SceneObjectParams SceneEditorApp::GetSelectedParams() const {
        SceneObjectParams buff;
        if (selectedObject != nullptr) {
            return selectedObject->params;
        }
        return buff;
    }

} // namspace drash
