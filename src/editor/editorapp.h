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
            CVec3f mSplitIntersection1;
            unsigned mSplitIntersection1Index = 0;
            CVec3f mSplitIntersection2;
            unsigned mSplitIntersection2Index = 0;
            unsigned int mSplitIntersectionsCount = 0;

            CFigure *mFigure = nullptr;
        };

      public:
        virtual bool Init() override;
        virtual void Step(double _dt) override;
        inline virtual void Render() override;
        virtual void Release() override;

        void StartBuild();

        inline bool IsStartBuild() const;

        bool BuildFigure(const std::string &_objectName);

        bool AddNewObjectToTemplate(const std::string &_name);

        void ShowObject(const std::string &_name);

        inline void SetCurrentTemplateName(const std::string &_name);
        inline void SetTreeRefreshHandler(const std::function<void()> &_han);
        inline void
        SetGetSelectedHandler(const std::function<std::string()> &_han);

        inline void ActiveMoveMode();
        inline void ActiveSplitFigureMode();
        inline void ActiveSplitObjectMode();
        inline void ActiveDeleteMode();
        void ActiveStretchMode();
        void ActiveMoveOfAxisMode();

        void SaveCurrentObject();

        inline greng::CCamera *GetCamera();

      private:
        float GetCurDepth();

        void SetProcessors();
        void SetCameraProcessors();
        void SetDragDrop();

        bool ValidateFigure();
        void RemoveCurrentObject();

        CFigure *SelectFigure(const CVec2f &_pos);

        void MoveFigure();

        void StretchFigure();

        void ChangeMode();
        void SelectVertex();
        void SettingCenterFigure();
        void MoveOfAxis();
        bool IsConvex() const;

      private:
        // Drag and Drop
        std::function<std::string()> mGetSelectedTemplateHandler = []() {
            return std::string("");
        };
        CSceneObjectGeometry *mDragTemplate = nullptr;
        bool mDragNow = false;
        void DrawDragTemplate();
        void ApplyDrop();

        // For current object
        CSceneObject *mCurrentObject = nullptr;
        State mState = Simple;
        std::string mCurrentTemplateName = "";
        std::function<void()> mTreeRefreshHandler = []() {};

        std::vector<drash::CVec2f> mVertexs;

        CFigure *mSelectedFigure = nullptr;

        CVec3f mOldPositionCursor = CVec3f(0);

        CVec2f mCurrentFigureVertex;

        CVec2f mCamRotFirstClick;

        int mVertexIndex = -1;

        bool mFrontSide = true;

        greng::CCamera *mCamera = nullptr;

        greng::CPointLight mPointLight;

        CMoveablePoint mMoveablePoint;

        CVec3f mOldCenterFigure;

        CTimer mTimer;

        static const float MOVING_SPEED;

        CColor4f mGridColor = CColor4f(0.8, 0.8, 0.8, 1);
        int mGridSegmentSize = 1;
        CVec2i mGridSize = CVec2i(20, 20);

        // for Split

        CVec3f mSplitMin;
        CVec3f mSplitMax;
        CPlane mSplitPlane;
        CVec3f mSplitPlanePoint1;
        CVec3f mSplitPlanePoint2;
        CVec3f mSplitPlanePoint3;
        CVec3f mSplitPlanePoint4;

        std::vector<SplitContext> mObjectContexts;
        SplitContext mSplitFigureContext;

        void BeginSplit();
        void DetectNewSplitPoint(const CVec2f &_p1, const CVec2f &_p2,
                                 unsigned int _index, const CRay &_r,
                                 SplitContext &_context) const;
        void ComputeIntersections(SplitContext &_context) const;
        void EndSplit();

        void RenderSplitPlane();
        void ComputeSplitPlanePoints();

        CRotationablePoint mRotationPoint;

        void SplitRotateStep(double _dt);
    };

    inline bool CObjectEditorApp::IsStartBuild() const {
        return mState == BuildState;
    }

    inline void CObjectEditorApp::ActiveMoveMode() {
        mState = MoveState;
        ChangeMode();
        //    LOG_INFO("Moving mode Active");
    }

    inline void CObjectEditorApp::ActiveMoveOfAxisMode() {
        mState = MoveOfAxisState;
        ChangeMode();
    }

    inline void
    CObjectEditorApp::SetCurrentTemplateName(const std::string &_name) {
        mCurrentTemplateName = _name;
    }

    inline void
    CObjectEditorApp::SetTreeRefreshHandler(const std::function<void()> &_han) {
        mTreeRefreshHandler = _han;
    }

    inline greng::CCamera *CObjectEditorApp::GetCamera() { return mCamera; }

    inline void CObjectEditorApp::ActiveSplitFigureMode() {
        mState = SplitFigureState;
        ChangeMode();
    }

    inline void CObjectEditorApp::ActiveSplitObjectMode() {
        mState = SplitObjectState;
        ChangeMode();
        BeginSplit();
    }

    inline void CObjectEditorApp::ActiveDeleteMode() {
        mState = DeleteFigure;
        ChangeMode();
    }

    inline void CObjectEditorApp::SetGetSelectedHandler(
        const std::function<std::string()> &_han) {
        mGetSelectedTemplateHandler = _han;
    }

} // namespace drash

#endif // CEDITORAPP_H
