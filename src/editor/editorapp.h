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

#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../app/app.h"
#include "../diag/timer.h"
#include "../greng/pointlight.h"
#include "../misc/moveablepoint.h"
#include "../misc/plane.h"
#include "../misc/rotationablepoint.h"

namespace drash {

    class ObjectEditorApp : public App {

    public:
        enum State {
            BuildState,
            MoveState,
            MoveOfAxisState,
            StretchState,
            DeleteFigure,
            SplitFigureState,
            SplitObjectState,
            Simple
        };

    private:
        struct SplitContext {
            Vec3f splitIntersection1;
            unsigned splitIntersection1Index = 0;
            Vec3f splitIntersection2;
            unsigned splitIntersection2Index = 0;
            unsigned int splitIntersectionsCount = 0;

            Figure* figure = nullptr;
        };

    public:
        ObjectEditorApp();

        virtual void Step(double _dt) override;
        inline virtual void Render() override;

        void StartBuild();

        inline bool IsStartBuild() const;

        bool BuildFigure(const std::string& _objectName);

        bool AddNewObjectToTemplate(const std::string& _name);

        void ShowObject(const std::string& _name);

        inline void SetCurrentTemplateName(const std::string& _name);
        inline void SetTreeRefreshHandler(const std::function<void()>& _han);
        inline void
        SetGetSelectedHandler(const std::function<std::string()>& _han);

        inline void ActiveMoveMode();
        inline void ActiveSplitFigureMode();
        inline void ActiveSplitObjectMode();
        inline void ActiveDeleteMode();
        void ActiveStretchMode();
        void ActiveMoveOfAxisMode();

        void SaveCurrentObject();

        inline greng::Camera* GetCamera();

    private:
        float GetCurDepth();

        void SetProcessors();
        void SetCameraProcessors();
        void SetDragDrop();

        bool ValidateFigure();
        void RemoveCurrentObject();

        Figure* SelectFigure(const Vec2f& _pos);

        void MoveFigure();

        void StretchFigure();

        void ChangeMode();
        void SelectVertex();
        void SettingCenterFigure();
        void MoveOfAxis();
        bool IsConvex() const;

    private:
        // Drag and Drop
        std::function<std::string()> getSelectedTemplateHandler = []() {
            return std::string("");
        };
        SceneObjectGeometry* dragTemplate = nullptr;
        bool dragNow = false;
        void DrawDragTemplate();
        void ApplyDrop();

        // For current object
        SceneObject* currentObject = nullptr;
        State state = Simple;
        std::string currentTemplateName = "";
        std::function<void()> treeRefreshHandler = []() {};

        std::vector<drash::Vec2f> vertexs;

        Figure* selectedFigure = nullptr;

        Vec3f oldPositionCursor = Vec3f(0);

        Vec2f currentFigureVertex;

        Vec2f camRotFirstClick;

        int vertexIndex = -1;

        bool frontSide = true;

        greng::Camera* camera = nullptr;

        greng::PointLight pointLight;

        MoveablePoint moveablePoint;

        Vec3f oldCenterFigure;

        Timer timer;

        static const float MOVING_SPEED;

        Color4f gridColor = Color4f(0.8, 0.8, 0.8, 1);
        int gridSegmentSize = 1;
        Vec2i gridSize = Vec2i(20, 20);

        // for Split

        Vec3f splitMin;
        Vec3f splitMax;
        Plane splitPlane;
        Vec3f splitPlanePoint1;
        Vec3f splitPlanePoint2;
        Vec3f splitPlanePoint3;
        Vec3f splitPlanePoint4;

        std::vector<SplitContext> objectContexts;
        SplitContext splitFigureContext;

        void BeginSplit();
        void DetectNewSplitPoint(const Vec2f& _p1, const Vec2f& _p2,
                                 unsigned int _index, const Ray& _r,
                                 SplitContext& _context) const;
        void ComputeIntersections(SplitContext& _context) const;
        void EndSplit();

        void RenderSplitPlane();
        void ComputeSplitPlanePoints();

        RotationablePoint rotationPoint;

        void SplitRotateStep(double _dt);
    };

    inline bool ObjectEditorApp::IsStartBuild() const {
        return state == BuildState;
    }

    inline void ObjectEditorApp::ActiveMoveMode() {
        state = MoveState;
        ChangeMode();
        //    LOG_INFO("Moving mode Active");
    }

    inline void ObjectEditorApp::ActiveMoveOfAxisMode() {
        state = MoveOfAxisState;
        ChangeMode();
    }

    inline void
    ObjectEditorApp::SetCurrentTemplateName(const std::string& _name) {
        currentTemplateName = _name;
    }

    inline void
    ObjectEditorApp::SetTreeRefreshHandler(const std::function<void()>& _han) {
        treeRefreshHandler = _han;
    }

    inline greng::Camera* ObjectEditorApp::GetCamera() { return camera; }

    inline void ObjectEditorApp::ActiveSplitFigureMode() {
        state = SplitFigureState;
        ChangeMode();
    }

    inline void ObjectEditorApp::ActiveSplitObjectMode() {
        state = SplitObjectState;
        ChangeMode();
        BeginSplit();
    }

    inline void ObjectEditorApp::ActiveDeleteMode() {
        state = DeleteFigure;
        ChangeMode();
    }

    inline void ObjectEditorApp::SetGetSelectedHandler(
        const std::function<std::string()>& _han) {
        getSelectedTemplateHandler = _han;
    }

} // namespace drash

#endif // EDITORAPP_H
