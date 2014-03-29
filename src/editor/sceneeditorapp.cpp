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

    CSceneEditorApp::CSceneEditorApp() {}

    CSceneEditorApp::CSceneEditorApp() {

        if (CApp::CApp() == false) {
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
        // GetGeometryManager().CreateSceneObjectFromTemplate("Object1",CSceneObjectParams());
    }

    void CSceneEditorApp::Step(double _dt) {
        timer.Tick();
        if (playLevel == true) {
            CApp::Step(_dt);
        } else {
            CApp::Step(0);
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

    void CSceneEditorApp::Render() {
        if (playLevel == true || paused == true) {
            CApp::Render();
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

    void CSceneEditorApp::UpdateTemplateSystem() {
        GetGeometryManager().Load();
    }

    bool CSceneEditorApp::LoadLevel(const std::string& _filename) {
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

    bool CSceneEditorApp::SaveLevelAs(const std::string& _filename) {
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

    bool CSceneEditorApp::NewLevel() {
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

    void CSceneEditorApp::StartCurrentLevel() {
        if (currentLevel != nullptr) {
            if (paused == false) {
                GetLevelManager().StartLevel(currentLevel);
            }

            playLevel = true;
            selectedObject = nullptr;
        }
    }

    void CSceneEditorApp::AddObject(const std::string& _name,
                                    const CVec3f& _pos) {
        if (currentLevel == nullptr) {
            LOG_WARN("Not set current level");
            return;
        }

        CSceneObjectGeometry* obj = GetGeometryManager().GetGeometry(_name);
        if (obj == nullptr) {
            LOG_ERR(_name.c_str() << " not found in Template System");
            return;
        }

        std::string obj_name = currentLevel->GetUniqueObjectName();

        CLevelObjectDesc* p = currentLevel->AddObject(_name, obj_name);
        if (p == nullptr) {
            return;
        }

        p->params.pos = _pos;
        p->geometryName = _name;
        p->levelObjectName = obj_name;

        treeRefreshHandler();
    }

    void CSceneEditorApp::SetProcessors() {
        GetEventSystem().SetProcessor(
            "RB", CAppEventProcessor([this]() {
                      if (playLevel == true) {
                          return;
                      }
                      selectedObject = SelectObject();
                      if (selectedObject != nullptr) {
                          oldpositon = selectedObject->params.pos;
                          moveablePoint.SetCenter(oldpositon);
                          rotationablePoint.SetPoint(oldpositon);
                          rotationablePoint.SetRotation(
                              CVec3f(0, 0, selectedObject->params.angle));
                      }
                  }));

        GetEventSystem().SetProcessor(
            "LB", CAppEventProcessor([this]() {
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

        GetEventSystem().SetProcessor("C-LB", CAppEventProcessor([this]() {

                                                  if (playLevel) {
                                                      return;
                                                  }
                                                  CLevelObjectDesc* temp =
                                                      SelectObject();
                                                  if (temp != nullptr) {
                                                      temp->params.dynamic =
                                                          !temp->params.dynamic;
                                                  }
                                              }));
    }

    void CSceneEditorApp::SetCameraProcessors() {
        GetEventSystem().SetProcessor(
            "MB",
            CAppEventProcessor([this]() { camRotFirstClick = GetCursorPos(); },
                               [this]() {
                CVec2f new_pos = GetCursorPos();

                CVec2f rot = camera->GetRotation().Get();
                rot.y -= new_pos.x - camRotFirstClick.x;
                rot.x += new_pos.y - camRotFirstClick.y;

                camera->GetRotation().Set(rot);

                camRotFirstClick = new_pos;
            }));

        GetEventSystem().SetProcessor(
            "w", CAppEventProcessor([this]() {},
                                    [this]() {
                                        camera->Forward(MOVING_SPEED *
                                                        timer.GetDeltaTime());
                                    },
                                    [this] {}));

        GetEventSystem().SetProcessor(
            "a", CAppEventProcessor([this]() {}, [this]() {
                     camera->Strafe(MOVING_SPEED * timer.GetDeltaTime());
                 }));

        GetEventSystem().SetProcessor(
            "s", CAppEventProcessor([this]() {}, [this]() {
                     camera->Forward(-MOVING_SPEED * timer.GetDeltaTime());
                 }));

        GetEventSystem().SetProcessor(
            "d", CAppEventProcessor([this]() {}, [this]() {
                     camera->Strafe(-MOVING_SPEED * timer.GetDeltaTime());
                 }));
    }

    void CSceneEditorApp::SetDragDropProcessors() {
        GetEventSystem().SetProcessor("DRL", CAppEventProcessor([this]() {
                                                 dragNow = false;
                                                 // dragTemplateName = "";
                                             }));

        GetEventSystem().SetProcessor(
            "DRDP", CAppEventProcessor([this]() {
                                           if (currentLevel != nullptr) {
                                               // qDebug() << "Proccess enter";
                                               dragTemplateName =
                                                   getSelectedTemplateHandler();
                                               dragNow = true;
                                           }
                                       },
                                       [this]() {}, [this]() {

                        if (dragNow == true) {
                            CPlane plane;
                            plane.SetNormal(CVec3f(0, 0, 1));
                            plane.SetPoint(CVec3f(0, 0, 0));

                            CVec3f position;
                            CVec2f cpos = GetCursorPos();
                            camera->CastRay(cpos, plane, position);
                            AddObject(dragTemplateName, position);

                            dragNow = false;
                            dragTemplateName = "";
                        }
                    }));
    }

    bool CSceneEditorApp::InitCamera() {
        greng::CCameraParams p;
        p.pos.Set(10, 10, 10.0f);
        p.rotation.Set(-M_PI / 4, M_PI / 4, 0);
        camera = GetGrengSystems().GetCameraManager().CreateCamera(p);

        if (camera == nullptr) {
            return false;
        }

        GetDebugRenderer().SetCamera(camera);

        return true;
    }

    bool CSceneEditorApp::InitPointLight() {
        light1.position.Set(0, 10, 0);
        GetDebugRenderer().SetLight(&light1);
        return true;
    }

    bool CSceneEditorApp::InitSpecialPoint() {
        moveablePoint.SetCamera(camera);

        rotationablePoint.SetRenderer(&GetGrengSystems().GetRenderer());

        rotationablePoint.SetCamera(camera);

        if (rotationablePoint.Init() == false) {
            return false;
        }

        rotationablePoint.SetAxisOX(false);
        rotationablePoint.SetAxisOY(false);

        return true;
    }

    void CSceneEditorApp::RatateObject() {
        if (selectedObject == nullptr) {
            return;
        }

        float anglez = rotationablePoint.GetRotation().z;
        selectedObject->params.angle = anglez;
        changedParams = true;
    }

    void CSceneEditorApp::RenderPoints() {
        if (selectedObject == nullptr) {
            return;
        }

        moveablePoint.Render(GetGrengSystems().GetRenderer());
        rotationablePoint.Render();
    }

    void CSceneEditorApp::RenderDragTemplate() {
        if (currentLevel == nullptr) {
            return;
        }
        if (dragNow == false) {
            return;
        }
        // qDebug() << dragTemplateName.c_str();
        if (dragTemplateName != "") {
            CSceneObjectGeometry* g =
                GetGeometryManager().GetGeometry(dragTemplateName);
            if (g == nullptr) {
                return;
            }

            CSceneObjectParams params;
            CVec3f position;
            CVec2f cpos = GetCursorPos();

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, 0));

            //        LOG_INFO(cpos);
            camera->CastRay(cpos, plane, position);
            //        v = vv;
            params.pos = position;
            //        LOG_INFO(position);
            GetDebugRenderer().RenderObject(*g, params);
        }
    }

    CLevelObjectDesc* CSceneEditorApp::SelectObject() {
        auto f = [this](unsigned int i) {
            return currentLevel->GetObjects()[i];
        };

        return GetDebugRenderer().FindObject(camera, GetCursorPos(), f,
                                             currentLevel->EnumObjects());
    }

    void CSceneEditorApp::MoveOfAxis() {
        selectedObject->params.pos = moveablePoint.GetCenter();
        oldpositon = moveablePoint.GetCenter();
        rotationablePoint.SetPoint(moveablePoint.GetCenter());
    }

    void CSceneEditorApp::StopLevel() {
        playLevel = false;
        ResetLevel();
        paused = false;
        //    GetLevelManager().StartLevel(currentLevel,objectParams);
    }

    void CSceneEditorApp::ResetLevel() {
        if (currentLevel != nullptr) {
            GetLevelManager().StartLevel(currentLevel);
            paused = false;
        }
    }

    void CSceneEditorApp::LookObject(const std::string& _geometryname,
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

    void CSceneEditorApp::SetDynamicParam(bool _val) {
        if (selectedObject != nullptr) {
            selectedObject->params.dynamic = _val;
        }
    }

    void CSceneEditorApp::SetFixedRotationParam(bool _val) {
        if (selectedObject != nullptr) {
            selectedObject->params.fixedRotation = _val;
        }
    }

    void CSceneEditorApp::SetAngleParams(float _angle) {
        if (selectedObject != nullptr) {
            selectedObject->params.angle = _angle;
        }
    }

    float CSceneEditorApp::GetAngleParams() const {
        if (IsObjectSelected() == true) {
            return selectedObject->params.angle;
        }
        return 0.0f;
    }

    CSceneObjectParams CSceneEditorApp::GetSelectedParams() const {
        CSceneObjectParams buff;
        if (selectedObject != nullptr) {
            return selectedObject->params;
        }
        return buff;
    }

} // namspace drash
