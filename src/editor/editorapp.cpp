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

#include "editorapp.h"

#include <QDebug>

#include "../greng/camera.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"
#include "../misc/ray.h"

using namespace greng;

namespace drash {

    // const
    const float ObjectEditorApp::MOVING_SPEED = 60.0f;

    ObjectEditorApp::ObjectEditorApp() {
        SetProcessors();
        SetCameraProcessors();
        SetDragDrop();
        GetGeometryManager().Load();

        CameraParams cp;
        cp.pos.Set(10, 10, 10.0f);
        cp.rotation.Set(-M_PI / 4, M_PI / 4, 0);
        camera = GetGrengSystems().GetCameraManager().CreateCamera(cp);
        GetDebugRenderer().SetCamera(camera);
        pointLight.position = GetCamera()->GetPos();

        GetDebugRenderer().SetLight(&pointLight);
        GetDebugRenderer().SetTexCoordsScale(0.5);

        treeRefreshHandler();

        moveablePoint.SetCamera(camera);
        rotationPoint.SetCamera(camera);
        rotationPoint.SetRenderer(&GetGrengSystems().GetRenderer());
        rotationPoint.Init();
        rotationPoint.SetAxisOX(false);
        rotationPoint.SetAxisOY(false);
        timer.Reset(true);

        LOG_INFO("EditorObject App initzializeted");
    }

    void ObjectEditorApp::Step(double _dt) {
        App::Step(0);

        timer.Tick();

        pointLight.position = GetCamera()->GetPos();

        if (state == MoveOfAxisState && selectedFigure != nullptr) {
            moveablePoint.SetCursorPos(GetCursorPos());
            moveablePoint.Step(_dt);
        }

        if (state == SplitFigureState || state == SplitObjectState) {
            SplitRotateStep(_dt);
        }
    }

    void ObjectEditorApp::Render() {
        Vec2i segments_counts(gridSize.x, gridSize.y);
        segments_counts /= gridSegmentSize;

        Vec2f grid_size_half(gridSize.x, gridSize.y);
        grid_size_half /= 2;

        for (unsigned int i = 1; i < (unsigned int)segments_counts.x; i++) {
            for (unsigned int j = 1; j < (unsigned int)segments_counts.y; j++) {
                Vec3f p1(i * gridSegmentSize, 0, j * gridSegmentSize);
                Vec3f p2(i * gridSegmentSize, 0, (j - 1) * gridSegmentSize);
                Vec3f p3((i - 1) * gridSegmentSize, 0,
                          (j - 1) * gridSegmentSize);
                Vec3f p4((i - 1) * gridSegmentSize, 0, j * gridSegmentSize);

                p1.x -= grid_size_half.x;
                p1.z -= grid_size_half.y;
                p2.x -= grid_size_half.x;
                p2.z -= grid_size_half.y;
                p3.x -= grid_size_half.x;
                p3.z -= grid_size_half.y;
                p4.x -= grid_size_half.x;
                p4.z -= grid_size_half.y;

                GetGrengSystems().GetRenderer().DrawLine(camera, p1, p2, 1,
                                                         gridColor, true);
                GetGrengSystems().GetRenderer().DrawLine(camera, p2, p3, 1,
                                                         gridColor, true);
                GetGrengSystems().GetRenderer().DrawLine(camera, p3, p4, 1,
                                                         gridColor, true);
                GetGrengSystems().GetRenderer().DrawLine(camera, p4, p1, 1,
                                                         gridColor, true);
            }
        }

        GetGrengSystems().GetRenderer().DrawLine(
            camera, Vec3f(-segments_counts.x * gridSegmentSize / 2, 0, 0),
            Vec3f(-1 + segments_counts.x * gridSegmentSize / 2, 0, 0), 2,
            Color4f(0, 0, 0, 1), false);

        GetGrengSystems().GetRenderer().DrawLine(
            camera, Vec3f(0, 0, -segments_counts.y * gridSegmentSize / 2),
            Vec3f(0, 0, -1 + segments_counts.y * gridSegmentSize / 2), 2,
            Color4f(0, 0, 0, 1), false);

        App::Render();

        if (vertexs.size() != 0 && state == BuildState) {
            for (unsigned int i = 1; i < vertexs.size(); i++) {
                GetGrengSystems().GetRenderer().DrawLine(
                    vertexs[i - 1], vertexs[i], 1, Color4f(0, 1, 0, 1));
            }
            GetGrengSystems().GetRenderer().DrawLine(
                vertexs[vertexs.size() - 1], GetCursorPos(), 1,
                Color4f(0, 1, 0, 1));
            GetGrengSystems().GetRenderer().DrawLine(vertexs[0], GetCursorPos(),
                                                     1, Color4f(0, 1, 0, 1));
        }
        if (state == StretchState && currentObject != nullptr) {
            for (unsigned int i = 0; i < currentObject->EnumFigures(); i++) {
                Figure* figure = currentObject->GetFigures()[i];
                for (unsigned int j = 0; j < figure->EnumVertices(); j++) {
                    Vec3f position(figure->GetVertices()[j],
                                    currentObject->GetPosZ() + figure->GetZ() +
                                        figure->GetDepth() * 0.5f);

                    Color4f color(1, 0.5, 0, 1);

                    Plane plane;
                    plane.SetNormal(Vec3f(0, 0, 1));
                    plane.SetPoint(position);

                    Vec3f cursor_pos;

                    camera->CastRay(GetCursorPos(), plane, cursor_pos);
                    if (drash::math::Abs(position.x - cursor_pos.x) <= 1 &&
                        drash::math::Abs(position.y - cursor_pos.y) <= 1) {
                        color.Col3().Set(255, 0, 0);
                    }

                    GetGrengSystems().GetRenderer().DrawPoint(camera, position,
                                                              10.0f, color);

                    color.Col3().Set(255, 155, 0);

                    position.z = currentObject->GetPosZ() + figure->GetZ() -
                                 figure->GetDepth() * 0.5f;

                    plane.SetPoint(position);

                    camera->CastRay(GetCursorPos(), plane, cursor_pos);

                    if (drash::math::Abs(position.x - cursor_pos.x) <= 1 &&
                        drash::math::Abs(position.y - cursor_pos.y) <= 1) {
                        color.Col3().Set(255, 0, 0);
                    }

                    GetGrengSystems().GetRenderer().DrawPoint(camera, position,
                                                              10.0f, color);
                }
            }
        }

        if (state == MoveOfAxisState && selectedFigure != nullptr) {
            moveablePoint.Render(GetGrengSystems().GetRenderer());
        }

        RenderSplitPlane();

        if (state == SplitObjectState) {
            rotationPoint.Render();
            moveablePoint.Render(GetGrengSystems().GetRenderer());
        }

        if (state == SplitFigureState && selectedFigure != nullptr) {
            rotationPoint.Render();
            moveablePoint.Render(GetGrengSystems().GetRenderer());
        }

        DrawDragTemplate();
    }

    void ObjectEditorApp::~ObjectEditorApp() { GetGeometryManager().Store(); }

    void ObjectEditorApp::StartBuild() {
        state = BuildState;
        ChangeMode();
    }

    void ObjectEditorApp::SetProcessors() {

        GetEventSystem().SetProcessor(
            "LB",
            AppEventProcessor(
                [this]() {
                    if (currentObject == nullptr) {
                        return;
                    }
                    switch (state) {
                    case BuildState: {
                        if (currentObject != nullptr)
                            vertexs.push_back(GetCursorPos());
                        if (IsConvex() == false) {
                            vertexs.pop_back();
                        }
                        break;
                    }
                    case MoveState: {
                        selectedFigure = SelectFigure(GetCursorPos());

                        Plane plane;
                        plane.SetPoint(Vec3f(0, 0, 0));
                        plane.SetNormal(Vec3f(0, 0, 1));

                        camera->CastRay(GetCursorPos(), plane,
                                        oldPositionCursor);
                        break;
                    }
                    case StretchState: {
                        SelectVertex();
                        break;
                    }
                    case MoveOfAxisState: {
                        if (selectedFigure != nullptr) {
                            moveablePoint.ClickBegin();
                        }
                        break;
                    }
                    case DeleteFigure: {
                        Figure* fig = SelectFigure(GetCursorPos());
                        if (fig != nullptr) {
                            currentObject->DestroyFigure(fig);
                            SaveCurrentObject();
                        }
                        break;
                    }
                    case SplitFigureState: {
                        rotationPoint.SetCursorPos(GetCursorPos());
                        rotationPoint.RotateBegin();
                        moveablePoint.ClickBegin();
                        break;
                    }
                    case SplitObjectState: {
                        rotationPoint.SetCursorPos(GetCursorPos());
                        rotationPoint.RotateBegin();
                        moveablePoint.ClickBegin();
                        break;
                    }
                    case Simple:
                        selectedFigure = nullptr;
                        break;
                    }
                },
                [this]() {
                    if (selectedFigure != nullptr && state == MoveState) {
                        MoveFigure();
                    }

                    if (selectedFigure != nullptr && state == MoveOfAxisState) {
                        moveablePoint.ClickPressing();
                        MoveOfAxis();
                    }

                    if (state == StretchState) {
                        StretchFigure();
                    }

                    if (state == SplitFigureState ||
                        state == SplitObjectState) {
                        moveablePoint.ClickPressing();
                    }
                },
                [this]() {
                    if (state == MoveState) {
                        selectedFigure = nullptr;
                        SaveCurrentObject();
                    }
                    if (state == MoveOfAxisState && selectedFigure != nullptr) {
                        moveablePoint.ClickEnd();
                        SaveCurrentObject();
                    }
                    if (state == StretchState) {
                        selectedFigure = nullptr;
                        SaveCurrentObject();
                        vertexIndex = -1;
                    }

                    if (state == SplitFigureState ||
                        state == SplitObjectState) {
                        rotationPoint.RotateEnd();
                        moveablePoint.ClickEnd();
                    }
                }));

        GetEventSystem().SetProcessor(
            "RB", AppEventProcessor([this]() {
                      switch (state) {
                      case BuildState:
                          BuildFigure(currentTemplateName);
                          break;
                      case MoveOfAxisState: {
                          selectedFigure = SelectFigure(GetCursorPos());
                          SettingCenterFigure();
                          break;
                      }
                      case SplitFigureState: {
                          selectedFigure = SelectFigure(GetCursorPos());
                          if (selectedFigure != nullptr) {
                              BeginSplit();
                          }
                          break;
                      }
                      case SplitObjectState:
                          break;
                      //            case SplitFigureState: {
                      //                if (selectedFigure != nullptr) {
                      //                    EndSplit();
                      //                }
                      //                break;
                      //            }
                      //            case SplitObjectState: {
                      //                EndSplit();
                      //                break;
                      //            }
                      case MoveState:
                          break;
                      case StretchState:
                          break;
                      case DeleteFigure:
                          break;
                      case Simple:
                          break;
                      }
                  }));

        GetEventSystem().SetProcessor(
            "WHUP", AppEventProcessor([this]() {
                        if (currentObject != nullptr) {
                            for (unsigned int i = 0;
                                 i < currentObject->EnumFigures(); i++) {
                                Figure* fig = currentObject->GetFigures()[i];
                                fig->SetDepth(fig->GetDepth() + 0.5);
                            }
                            SaveCurrentObject();
                        }
                    }));
        GetEventSystem().SetProcessor(
            "WHDN", AppEventProcessor([this]() {
                        if (currentObject != nullptr) {
                            for (unsigned int i = 0;
                                 i < currentObject->EnumFigures(); i++) {
                                Figure* fig = currentObject->GetFigures()[i];
                                if (fig->GetDepth() - 0.5 > 0.01) {
                                    fig->SetDepth(fig->GetDepth() - 0.5);
                                }
                            }
                            SaveCurrentObject();
                        }
                    }));
        GetEventSystem().SetProcessor("SPC",
                                      AppEventProcessor([this]() {
                                          if (state == SplitFigureState ||
                                              state == SplitObjectState) {
                                              EndSplit();
                                          }
                                      }));
    }

    void ObjectEditorApp::SetCameraProcessors() {

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

    void ObjectEditorApp::SetDragDrop() {
        GetEventSystem().SetProcessor("DRL", AppEventProcessor([this]() {
                                                 dragNow = false;
                                                 dragTemplate = nullptr;
                                             }));

        GetEventSystem().SetProcessor(
            "DRDP",
            AppEventProcessor([this]() {
                                   if (currentObject == nullptr) {
                                       return;
                                   }
                                   //        qDebug() << "Proccess enter";
                                   dragTemplate =
                                       GetGeometryManager().GetGeometry(
                                           getSelectedTemplateHandler());
                                   if (dragTemplate != nullptr) {
                                       dragNow = true;
                                   } else {
                                       dragNow = false;
                                   }
                               },
                               [this]() {}, [this]() {

                if (dragNow == true) {
                    ApplyDrop();
                    dragNow = false;
                }
            }));
    }

    bool ObjectEditorApp::BuildFigure(const std::string& _objectName) {
        if (vertexs.size() < 3 || currentObject == nullptr) {
            return false;
        }
        if (ValidateFigure() == false) {
            std::reverse(vertexs.begin(), vertexs.end());
            if (ValidateFigure() == false) {
                LOG_ERR("This figure can't build, canceled");
                return false;
            }
        }

        auto obj = GetGeometryManager().GetGeometry(_objectName);
        if (obj == nullptr) {
            return false;
        }

        FigureParams param;
        std::for_each(vertexs.begin(), vertexs.end(), [this](Vec2f& v) {
            Plane plane;
            plane.SetNormal(Vec3f(0, 0, 1));
            plane.SetPoint(Vec3f(0, 0, 0));

            Vec3f vv;
            camera->CastRay(v, plane, vv);
            v = vv;
        });
        param.vertices = vertexs;
        obj->figures.push_back(param);

        ShowObject(_objectName);

        treeRefreshHandler();

        ActiveMoveMode();
        return true;
    }

    bool ObjectEditorApp::AddNewObjectToTemplate(const std::string& _name) {
        if (GetGeometryManager().CreateGeometry(_name) == nullptr) {
            return false;
        }
        ShowObject(_name);
        return true;
    }

    void ObjectEditorApp::ShowObject(const std::string& _name) {
        RemoveCurrentObject();
        SetCurrentTemplateName(_name);
        SceneObjectParams params;
        auto obj = GetGeometryManager().CreateSceneObject(_name, params);
        currentObject = obj;
    }

    bool ObjectEditorApp::ValidateFigure() {
        if (vertexs.size() < 3) {
            return false;
        }

        /// Box2d code

        float area = 0.0f;

        Vec2f pRef(0.0f, 0.0f);

        std::vector<drash::Vec2f>& vs = vertexs;

        for (unsigned int i = 0; i < vs.size(); ++i) {
            Vec2f p1 = pRef;
            Vec2f p2 = vs[i];
            Vec2f p3 = i + 1 < vs.size() ? vs[i + 1] : vs[0];

            Vec2f e1 = p2 - p1;
            Vec2f e2 = p3 - p1;
            float D = e1.x * e2.y - e1.y * e2.x;
            float triangleArea = 0.5f * D;
            area += triangleArea;
        }
        const float espilion = 1.19209289550781250000e-7F;

        if (area <= espilion) {
            return false;
        }

        return true;
    }

    void ObjectEditorApp::RemoveCurrentObject() {
        if (currentObject != nullptr) {
            GetScene().DestroyObject(currentObject);
            currentObject = nullptr;
        }
    }

    Figure* ObjectEditorApp::SelectFigure(const Vec2f& _pos) {
        if (currentObject != nullptr) {
            return GetDebugRenderer().FindFigure(camera, _pos);
        }

        return nullptr;
    }

    void ObjectEditorApp::MoveFigure() {
        if (selectedFigure == nullptr) {
            return;
        }

        Vec3f pos;

        Plane plane;
        plane.SetNormal(Vec3f(0, 0, 1));
        plane.SetPoint(Vec3f(0, 0, 0));

        camera->CastRay(GetCursorPos(), plane, pos);

        float disX = pos.x - oldPositionCursor.x;
        float disY = pos.y - oldPositionCursor.y;

        oldPositionCursor = pos;

        const Vec2f* v = selectedFigure->GetVertices();
        Vec2f* new_vertices = new Vec2f[selectedFigure->EnumVertices()];
        for (unsigned int i = 0; i < selectedFigure->EnumVertices(); i++) {
            new_vertices[i] = v[i];
            new_vertices[i].x += disX;
            new_vertices[i].y += disY;
        }

        selectedFigure->SetVertices(new_vertices,
                                    selectedFigure->EnumVertices());
        delete[] new_vertices;
    }

    void ObjectEditorApp::StretchFigure() {
        if (vertexIndex == -1) {
            return;
        }
        Vec2f* ver = new Vec2f[selectedFigure->EnumVertices()];
        for (unsigned int i = 0; i < selectedFigure->EnumVertices(); i++) {
            if (i == (unsigned int)vertexIndex) {
                Vec2f posCur = GetCursorPos();
                // qDebug() << posCur.x << " " << posCur.y;

                float tmp = selectedFigure->GetDepth() * 0.5;

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(Vec3f(0, 0, frontSide ? tmp : -tmp));

                Vec3f pos;

                camera->CastRay(posCur, plane, pos);

                ver[i].Set(pos.x, pos.y);
            } else {
                ver[i] = selectedFigure->GetVertices()[i];
            }
        }
        selectedFigure->SetVertices(ver, selectedFigure->EnumVertices());
        delete[] ver;
    }

    void ObjectEditorApp::ChangeMode() {
        vertexs.clear();
        selectedFigure = nullptr;
        SaveCurrentObject();
        objectContexts.clear();

        std::string logstr = "";

        switch (state) {
        case MoveState:
            logstr = "Moving";
            break;
        case StretchState:
            logstr = "Stretch";
            break;
        case MoveOfAxisState:
            logstr = "Moving of Axis";
            break;
        case BuildState:
            logstr = "Building";
            break;
        case DeleteFigure:
            logstr = "Deleting";
            break;
        case SplitFigureState:
            logstr = "Spliting figure";
            break;
        case SplitObjectState:
            logstr = "Spliting object";
            break;
        default:
            break;
        }

        if (logstr != "") {
            LOG_INFO(logstr << " mode active");
        }
    }

    void ObjectEditorApp::SelectVertex() {
        for (unsigned int i = 0; i < currentObject->EnumFigures(); i++) {
            Figure* figure = currentObject->GetFigures()[i];

            for (unsigned int j = 0; j < figure->EnumVertices(); j++) {
                Vec3f position(figure->GetVertices()[j],
                                currentObject->GetPosZ() + figure->GetZ() +
                                    figure->GetDepth() * 0.5f);

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(position);

                Vec3f cursor_pos;

                camera->CastRay(GetCursorPos(), plane, cursor_pos);

                if (drash::math::Abs(position.x - cursor_pos.x) <= 1.0f &&
                    drash::math::Abs(position.y - cursor_pos.y) <= 1.0f) {
                    currentFigureVertex = position;
                    vertexIndex = j;
                    selectedFigure = figure;
                    frontSide = true;
                    break;
                } else {
                    position.z = currentObject->GetPosZ() + figure->GetZ() -
                                 figure->GetDepth() * 0.5f;

                    plane.SetPoint(position);

                    camera->CastRay(GetCursorPos(), plane, cursor_pos);

                    if (drash::math::Abs(position.x - cursor_pos.x) <= 1.0f &&
                        drash::math::Abs(position.y - cursor_pos.y) <= 1.0f) {
                        currentFigureVertex = position;
                        vertexIndex = j;
                        selectedFigure = figure;
                        frontSide = false;
                        break;
                    }
                }
            }
        }
    }

    void ObjectEditorApp::SettingCenterFigure() {
        if (selectedFigure == nullptr) {
            return;
        }

        Vec3f center;

        center.Set(selectedFigure->GetVertices()[0],
                   currentObject->GetPosZ() + selectedFigure->GetZ());

        for (unsigned int i = 1; i < selectedFigure->EnumVertices(); i++) {
            center.AsVec2() += selectedFigure->GetVertices()[i];
        }

        center.AsVec2() /= Vec2f(selectedFigure->EnumVertices());

        moveablePoint.SetCenter(center);
        moveablePoint.SetAxisOX(true);
        moveablePoint.SetAxisOY(true);
        moveablePoint.SetAxisOZ(true);
        oldCenterFigure = center;
    }

    void ObjectEditorApp::MoveOfAxis() {
        if (selectedFigure == nullptr) {
            return;
        }
        Vec3f pos;

        Plane plane;
        plane.SetNormal(Vec3f(0, 0, 1));
        plane.SetPoint(Vec3f(0, 0, 0));

        camera->CastRay(GetCursorPos(), plane, pos);
        Vec3f newCenter = moveablePoint.GetCenter();
        float disX = newCenter.x - oldCenterFigure.x;
        float disY = newCenter.y - oldCenterFigure.y;
        oldPositionCursor = pos;

        const Vec2f* v = selectedFigure->GetVertices();
        Vec2f* new_vertices = new Vec2f[selectedFigure->EnumVertices()];
        for (unsigned int i = 0; i < selectedFigure->EnumVertices(); i++) {
            new_vertices[i] = v[i];
            new_vertices[i].x += disX;
            new_vertices[i].y += disY;
        }

        selectedFigure->SetVertices(new_vertices,
                                    selectedFigure->EnumVertices());
        delete[] new_vertices;

        selectedFigure->SetZ(selectedFigure->GetZ() + newCenter.z -
                             oldCenterFigure.z);
        oldCenterFigure = newCenter;
    }

    bool ObjectEditorApp::IsConvex() const {
        if (vertexs.size() <= 3)
            return true;

        int res = math::Sign(
            (vertexs[1].x - vertexs[0].x) * (vertexs[2].y - vertexs[0].y) -
            (vertexs[2].x - vertexs[0].x) * (vertexs[1].y - vertexs[0].y));

        const int N = vertexs.size();

        for (int i = 1; i < N; i++) {
            float buf = (vertexs[(i + 1) % N].x - vertexs[i].x) *
                            (vertexs[(i + 2) % N].y - vertexs[i].y) -
                        (vertexs[(i + 2) % N].x - vertexs[i].x) *
                            (vertexs[(i + 1) % N].y - vertexs[i].y);
            if (res != math::Sign(buf)) {
                return false;
            }
        }

        return true;
    }

    void ObjectEditorApp::ActiveStretchMode() {
        state = StretchState;
        ChangeMode();
    }

    void ObjectEditorApp::SaveCurrentObject() {
        if (currentObject == nullptr) {
            return;
        }

        SceneObjectGeometry* geometry = new SceneObjectGeometry();
        currentObject->DumpGeometry(geometry);

        SceneObjectGeometry* g =
            GetGeometryManager().GetGeometry(currentTemplateName);
        g->destructionGraph = geometry->destructionGraph;
        g->figures = geometry->figures;

        ShowObject(currentTemplateName);
    }

    float ObjectEditorApp::GetCurDepth() {
        float depth = drash::math::Abs(currentObject->GetPosZ() -
                                       camera->GetPos().Get().z);
        return depth;
    }

    // For Split mode

    float Area(const Vec2f& _p1, const Vec2f& _p2, const Vec2f& _p3) {
        return (_p2.x - _p1.x) * (_p3.y - _p1.y) -
               (_p2.y - _p1.y) * (_p3.x - _p1.x);
    }

    float Intersect_1(float _x, float _y, float _a, float _b) {
        if (_x > _y)
            std::swap(_x, _y);
        if (_a > _b)
            std::swap(_a, _b);
        return math::Max(_x, _a) <= math::Min(_y, _b);
    }

    bool Intersect(const Vec2f& _p1, const Vec2f& _p2, const Vec2f& _p3,
                   const Vec2f& _p4) {
        return Intersect_1(_p1.x, _p2.x, _p3.x, _p4.x) &&
               Intersect_1(_p1.y, _p2.y, _p3.y, _p4.y) &&
               Area(_p1, _p2, _p3) * Area(_p1, _p2, _p4) <= 0 &&
               Area(_p3, _p4, _p1) * Area(_p3, _p4, _p2) <= 0;
    }

    void ObjectEditorApp::BeginSplit() {
        if (state == SplitFigureState) {
            if (currentObject != nullptr && selectedFigure != nullptr) {

                splitMin.Set(selectedFigure->GetVertices()[0].x,
                             selectedFigure->GetVertices()[0].y,
                             currentObject->GetPosZ() + selectedFigure->GetZ());
                splitMax.Set(selectedFigure->GetVertices()[0].x,
                             selectedFigure->GetVertices()[0].y,
                             currentObject->GetPosZ() + selectedFigure->GetZ());

                for (unsigned int i = 1; i < selectedFigure->EnumVertices();
                     i++) {
                    splitMin.x = math::Min<float>(
                        splitMin.x, selectedFigure->GetVertices()[i].x);
                    splitMax.x = math::Max<float>(
                        splitMax.x, selectedFigure->GetVertices()[i].x);
                    splitMin.y = math::Min<float>(
                        splitMin.y, selectedFigure->GetVertices()[i].y);
                    splitMax.y = math::Max<float>(
                        splitMax.y, selectedFigure->GetVertices()[i].y);
                    splitMin.z = math::Min<float>(splitMin.z,
                                                  currentObject->GetPosZ() +
                                                      selectedFigure->GetZ());
                    splitMax.z = math::Max<float>(splitMax.z,
                                                  currentObject->GetPosZ() +
                                                      selectedFigure->GetZ());
                }

                splitMin.z -= selectedFigure->GetDepth() * 0.5;
                splitMax.z += selectedFigure->GetDepth() * 0.5;
                splitMin -= 1;
                splitMax += 1;

                splitPlane.SetNormal(Vec3f(0, 1, 0));
                splitPlane.SetPoint(Vec3f(0.5f * (splitMin.x + splitMax.x),
                                           0.5f * (splitMin.y + splitMax.y),
                                           0.5f * (splitMin.z + splitMax.z)));

                ComputeSplitPlanePoints();
                splitFigureContext.figure = selectedFigure;
                objectContexts.push_back(splitFigureContext);
                ComputeIntersections(splitFigureContext);
            }
        } else {
            //--
            if (currentObject != nullptr) {
                if (currentObject->EnumFigures() != 0) {

                    splitMin.Set(
                        currentObject->GetFigures()[0]->GetVertices()[0].x,
                        currentObject->GetFigures()[0]->GetVertices()[0].y,
                        currentObject->GetPosZ() +
                            currentObject->GetFigures()[0]->GetZ());
                    splitMax.Set(
                        currentObject->GetFigures()[0]->GetVertices()[0].x,
                        currentObject->GetFigures()[0]->GetVertices()[0].y,
                        currentObject->GetPosZ() +
                            currentObject->GetFigures()[0]->GetZ());

                    float maxDepth = currentObject->GetFigures()[0]->GetDepth();

                    for (unsigned int i = 0; i < currentObject->EnumFigures();
                         i++) {
                        Figure* figure = currentObject->GetFigures()[i];
                        for (unsigned int i = 1; i < figure->EnumVertices();
                             i++) {
                            splitMin.x = math::Min<float>(
                                splitMin.x, figure->GetVertices()[i].x);
                            splitMax.x = math::Max<float>(
                                splitMax.x, figure->GetVertices()[i].x);
                            splitMin.y = math::Min<float>(
                                splitMin.y, figure->GetVertices()[i].y);
                            splitMax.y = math::Max<float>(
                                splitMax.y, figure->GetVertices()[i].y);
                            splitMin.z = math::Min<float>(
                                splitMin.z,
                                currentObject->GetPosZ() + figure->GetZ());
                            splitMax.z = math::Max<float>(
                                splitMax.z,
                                currentObject->GetPosZ() + figure->GetZ());
                        }
                        maxDepth =
                            math::Max<float>(maxDepth, figure->GetDepth());
                        SplitContext context;
                        context.figure = figure;
                        objectContexts.push_back(context);
                    }
                    splitMin.z -= maxDepth * 0.5;
                    splitMax.z += maxDepth * 0.5;
                    splitMin -= 1;
                    splitMax += 1;

                    splitPlane.SetNormal(Vec3f(0, 1, 0));
                    splitPlane.SetPoint(
                        Vec3f(0.5f * (splitMin.x + splitMax.x),
                               0.5f * (splitMin.y + splitMax.y),
                               0.5f * (splitMin.z + splitMax.z)));

                    ComputeSplitPlanePoints();
                    for (SplitContext& context : objectContexts) {
                        ComputeIntersections(context);
                    }
                }
            }
            //--
        }
        rotationPoint.SetPoint(splitPlane.GetPoint());
        rotationPoint.SetRotation(Vec3f(0.0f, 0.0f, 0.0f));

        moveablePoint.SetCenter(splitPlane.GetPoint());

        moveablePoint.SetAxisOX(true);
        moveablePoint.SetAxisOY(true);
        moveablePoint.SetAxisOZ(false);
    }

    void ObjectEditorApp::DetectNewSplitPoint(const Vec2f& _p1,
                                               const Vec2f& _p2,
                                               unsigned int _index,
                                               const Ray& _r,
                                               SplitContext& _context) const {
        if (currentObject == nullptr) {
            return;
        }

        float centerz = currentObject->GetPosZ() + _context.figure->GetZ();

        Plane p;
        p.Set(Vec3f(_p1, centerz), Vec3f(_p2, centerz),
              Vec3f(_p2, centerz - 1));

        Vec3f p1;
        Vec3f p2;
        p.CastRay(_r, p1);
        p2 = p1;

        if (_context.splitIntersectionsCount == 0) {
            _context.splitIntersection1 = p1;
            _context.splitIntersection1Index = _index;
        } else if (_context.splitIntersectionsCount == 1) {
            _context.splitIntersection2 = p1;
            _context.splitIntersection2Index = _index;
        }

        if (Intersect(_p1, _p2, splitPlanePoint4, splitPlanePoint1)) {
            _context.splitIntersectionsCount++;
        }
    }

    void ObjectEditorApp::ComputeSplitPlanePoints() {
        Ray r;

        splitPlanePoint1.Set(splitMin.x, 0, splitMax.z);
        splitPlanePoint2.Set(splitMin.x, 0, splitMin.z);
        splitPlanePoint3.Set(splitMax.x, 0, splitMin.z);
        splitPlanePoint4.Set(splitMax.x, 0, splitMax.z);

        r.SetDirection(Vec3f(0, -1, 0));

        r.SetPoint(splitPlanePoint1);
        splitPlane.CastRay(r, splitPlanePoint1);
        r.SetPoint(splitPlanePoint2);
        splitPlane.CastRay(r, splitPlanePoint2);
        r.SetPoint(splitPlanePoint3);
        splitPlane.CastRay(r, splitPlanePoint3);
        r.SetPoint(splitPlanePoint4);
        splitPlane.CastRay(r, splitPlanePoint4);
    }

    void ObjectEditorApp::ComputeIntersections(SplitContext& _context) const {

        if (_context.figure != nullptr) {
            Vec3f dir = splitPlanePoint1;
            dir -= splitPlanePoint4;

            float centerz = currentObject->GetPosZ() + _context.figure->GetZ();

            Ray r;
            r.SetPoint(Vec3f(splitPlanePoint4.AsVec2(), centerz));
            r.SetDirection(dir);

            _context.splitIntersectionsCount = 0;

            if (_context.figure->EnumVertices() != 0) {
                for (unsigned int i = 1; i < _context.figure->EnumVertices();
                     i++) {
                    DetectNewSplitPoint(_context.figure->GetVertices()[i - 1],
                                        _context.figure->GetVertices()[i],
                                        i - 1, r, _context);
                }
                DetectNewSplitPoint(_context.figure->GetVertices()
                                        [_context.figure->EnumVertices() - 1],
                                    _context.figure->GetVertices()[0],
                                    _context.figure->EnumVertices() - 1, r,
                                    _context);
            }
        }
    }

    void ObjectEditorApp::EndSplit() {

        if (state == SplitFigureState) {
            objectContexts.clear();
            objectContexts.push_back(splitFigureContext);
        }

        for (SplitContext& context : objectContexts) {

            if (context.figure == nullptr) {
                continue;
            }

            if (context.splitIntersectionsCount == 2) {
                unsigned int fsize = context.splitIntersection1Index + 2 +
                                     context.figure->EnumVertices() -
                                     context.splitIntersection2Index;

                FigureParams fp;
                fp.vertices.resize(fsize);
                fp.depth = context.figure->GetDepth();
                fp.z = context.figure->GetZ();

                unsigned int i = 0;

                for (; i <= context.splitIntersection1Index; i++) {
                    fp.vertices[i] = context.figure->GetVertices()[i];
                }

                fp.vertices[i++] = context.splitIntersection1;
                fp.vertices[i++] = context.splitIntersection2;
                for (i = context.splitIntersection2Index + 1;
                     i < context.figure->EnumVertices(); i++) {
                    fp.vertices[i] = context.figure->GetVertices()[i];
                }

                currentObject->CreateFigure(fp);

                fsize = context.splitIntersection2Index -
                        context.splitIntersection1Index + 2;

                fp.vertices.clear();
                fp.vertices.resize(fsize);

                i = 0;
                fp.vertices[i++] = context.splitIntersection1;
                for (unsigned int j = context.splitIntersection1Index + 1;
                     j <= context.splitIntersection2Index; j++) {
                    fp.vertices[i++] = context.figure->GetVertices()[j];
                }
                fp.vertices[i] = context.splitIntersection2;

                currentObject->CreateFigure(fp);

                currentObject->DestroyFigure(context.figure);

                SceneObjectGeometry* geometry = new SceneObjectGeometry();
                currentObject->DumpGeometry(geometry);

                SceneObjectGeometry* g =
                    GetGeometryManager().GetGeometry(currentTemplateName);
                g->destructionGraph = geometry->destructionGraph;
                g->figures = geometry->figures;
            }
        }
        treeRefreshHandler();
    }

    void ObjectEditorApp::RenderSplitPlane() {
        if ((state != SplitFigureState && state != SplitObjectState) ||
            currentObject == nullptr) {
            return;
        }

        if (state == SplitFigureState && selectedFigure == nullptr) {
            return;
        }

        GetGrengSystems().GetRenderer().DrawTriangle(
            GetCamera(), splitPlanePoint1, splitPlanePoint2, splitPlanePoint4,
            Color4f(1, 0, 0.5, 0.5), true);
        GetGrengSystems().GetRenderer().DrawTriangle(
            GetCamera(), splitPlanePoint4, splitPlanePoint2, splitPlanePoint3,
            Color4f(1, 0, 0.5, 0.5), true);

        for (SplitContext& context : objectContexts) {
            if (context.splitIntersectionsCount == 2) {
                auto draw_split = [&](Vec3f _split_intersection) {
                    Vec3f p1 = _split_intersection;
                    Vec3f p2 = _split_intersection;

                    p1.z = currentObject->GetPosZ() + context.figure->GetZ() -
                           context.figure->GetDepth() * 0.5f;
                    p2.z = currentObject->GetPosZ() + context.figure->GetZ() +
                           context.figure->GetDepth() * 0.5f;

                    GetGrengSystems().GetRenderer().DrawLine(
                        GetCamera(), p1, p2, 2, Color4f(1, 1, 1), false);
                };

                draw_split(context.splitIntersection1);
                draw_split(context.splitIntersection2);
            }
        }
    }

    void ObjectEditorApp::SplitRotateStep(double _dt) {
        if (state == SplitFigureState && selectedFigure == nullptr) {
            return;
        }
        rotationPoint.SetCursorPos(GetCursorPos());
        rotationPoint.Step(_dt);
        moveablePoint.SetCursorPos(GetCursorPos());
        moveablePoint.Step(_dt);

        rotationPoint.SetPoint(moveablePoint.GetCenter());
        float angle = rotationPoint.GetRotation().z;
        double cs = cos(M_PI / 2 + angle);
        double ss = sin(M_PI / 2 + angle);
        splitPlane.SetNormal(Vec3f(cs, ss, 0));
        splitPlane.SetPoint(moveablePoint.GetCenter());
        ComputeSplitPlanePoints();
        if (state == SplitFigureState) {
            ComputeIntersections(splitFigureContext);
        } else {
            for (SplitContext& context : objectContexts) {
                ComputeIntersections(context);
            }
        }
    }

    void ObjectEditorApp::DrawDragTemplate() {
        if (dragNow == false || dragTemplate == nullptr) {
            return;
        }
        //    qDebug() << "Draw drag Template";
        SceneObjectParams params;
        Vec3f position;
        Vec2f cpos = GetCursorPos();

        Plane plane;
        plane.SetNormal(Vec3f(0, 0, 1));
        plane.SetPoint(Vec3f(0, 0, 0));

        camera->CastRay(cpos, plane, position);
        params.pos = position;
        GetDebugRenderer().RenderObject(*dragTemplate, params);
    }

    void ObjectEditorApp::ApplyDrop() {
        if (dragTemplate == nullptr) {
            return;
        }

        //    SceneObjectGeometry a;
        /*
         *    std::vector<FigureParams> figures;
        std::vector<std::vector<unsigned int>> destructionGraph;
         */

        Plane plane;
        plane.SetNormal(Vec3f(0, 0, 1));
        plane.SetPoint(Vec3f(0, 0, 0));

        Vec3f position;
        Vec2f cpos = GetCursorPos();
        camera->CastRay(cpos, plane, position);

        SceneObjectGeometry* currg =
            GetGeometryManager().GetGeometry(currentTemplateName);

        for (FigureParams figure : dragTemplate->figures) {
            for (auto& v : figure.vertices) {
                v.Set(v.x + position.x, v.y + position.y);
                qDebug() << v.x << " " << v.y;
            }
            //        qDebug() << "add figure";
            currg->figures.push_back(figure);
        }
        qDebug() << currentTemplateName.c_str();
        ShowObject(currentTemplateName);
        treeRefreshHandler();
    }

} // namespace drash
