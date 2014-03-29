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

#ifndef CEDITORAPP_H
#define CEDITORAPP_H

#include "../app/app.h"
#include "../diag/timer.h"
#include "../greng/pointlight.h"
#include "../misc/moveablepoint.h"
#include "../misc/plane.h"
#include "../misc/rotationablepoint.h"

namespace drash {

    class CObjectEditorApp : public CApp {

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
            CVec3f splitIntersection1;
            unsigned splitIntersection1Index = 0;
            CVec3f splitIntersection2;
            unsigned splitIntersection2Index = 0;
            unsigned int splitIntersectionsCount = 0;

            CFigure* figure = nullptr;
        };

      public:
        CObjectEditorApp();

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

        inline greng::CCamera* GetCamera();

      private:
        float GetCurDepth();

        void SetProcessors();
        void SetCameraProcessors();
        void SetDragDrop();

        bool ValidateFigure();
        void RemoveCurrentObject();

        CFigure* SelectFigure(const CVec2f& _pos);

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
        CSceneObjectGeometry* dragTemplate = nullptr;
        bool dragNow = false;
        void DrawDragTemplate();
        void ApplyDrop();

        // For current object
        CSceneObject* currentObject = nullptr;
        State state = Simple;
        std::string currentTemplateName = "";
        std::function<void()> treeRefreshHandler = []() {};

        std::vector<drash::CVec2f> vertexs;

        CFigure* selectedFigure = nullptr;

        CVec3f oldPositionCursor = CVec3f(0);

        CVec2f currentFigureVertex;

        CVec2f camRotFirstClick;

        int vertexIndex = -1;

        bool frontSide = true;

        greng::CCamera* camera = nullptr;

        greng::CPointLight pointLight;

        CMoveablePoint moveablePoint;

        CVec3f oldCenterFigure;

        CTimer timer;

        static const float MOVING_SPEED;

        CColor4f gridColor = CColor4f(0.8, 0.8, 0.8, 1);
        int gridSegmentSize = 1;
        CVec2i gridSize = CVec2i(20, 20);

        // for Split

        CVec3f splitMin;
        CVec3f splitMax;
        CPlane splitPlane;
        CVec3f splitPlanePoint1;
        CVec3f splitPlanePoint2;
        CVec3f splitPlanePoint3;
        CVec3f splitPlanePoint4;

        std::vector<SplitContext> objectContexts;
        SplitContext splitFigureContext;

        void BeginSplit();
        void DetectNewSplitPoint(const CVec2f& _p1, const CVec2f& _p2,
                                 unsigned int _index, const CRay& _r,
                                 SplitContext& _context) const;
        void ComputeIntersections(SplitContext& _context) const;
        void EndSplit();

        void RenderSplitPlane();
        void ComputeSplitPlanePoints();

        CRotationablePoint rotationPoint;

        void SplitRotateStep(double _dt);
    };

    inline bool CObjectEditorApp::IsStartBuild() const {
        return state == BuildState;
    }

    inline void CObjectEditorApp::ActiveMoveMode() {
        state = MoveState;
        ChangeMode();
        //    LOG_INFO("Moving mode Active");
    }

    inline void CObjectEditorApp::ActiveMoveOfAxisMode() {
        state = MoveOfAxisState;
        ChangeMode();
    }

    inline void
    CObjectEditorApp::SetCurrentTemplateName(const std::string& _name) {
        currentTemplateName = _name;
    }

    inline void
    CObjectEditorApp::SetTreeRefreshHandler(const std::function<void()>& _han) {
        treeRefreshHandler = _han;
    }

    inline greng::CCamera* CObjectEditorApp::GetCamera() { return camera; }

    inline void CObjectEditorApp::ActiveSplitFigureMode() {
        state = SplitFigureState;
        ChangeMode();
    }

    inline void CObjectEditorApp::ActiveSplitObjectMode() {
        state = SplitObjectState;
        ChangeMode();
        BeginSplit();
    }

    inline void CObjectEditorApp::ActiveDeleteMode() {
        state = DeleteFigure;
        ChangeMode();
    }

    inline void CObjectEditorApp::SetGetSelectedHandler(
        const std::function<std::string()>& _han) {
        getSelectedTemplateHandler = _han;
    }

} // namespace drash

#endif // CEDITORAPP_H
