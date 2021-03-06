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

#ifndef SCENEEDITORAPP_H
#define SCENEEDITORAPP_H

#include "../app/app.h"

#include "../diag/timer.h"
#include "../greng/pointlight.h"
#include "../misc/moveablepoint.h"
#include "../misc/rotationablepoint.h"

namespace drash {

    class SceneEditorApp : public App {
    public:
        SceneEditorApp();

        virtual void Step(double _dt) override;
        virtual void Render() override;

        void UpdateTemplateSystem();

        bool LoadLevel(const std::string& _filename);

        bool SaveLevelAs(const std::string& _filename);

        bool NewLevel();

        void StartCurrentLevel();

        void AddObject(const std::string& _name, const Vec3f& _pos);

        //
        inline bool SaveLevel();

        inline bool IsSetFileName() const;
        inline bool IsSetLevel() const;
        inline LevelDesc* GetCurrentLevel() const;
        inline std::string GetLevelFileName() const;

        inline void SetTreeRefreshHandler(const std::function<void()>& _han);
        inline void
        SetGetSelectedHandler(const std::function<std::string()>& _han);
        inline void PauseLevel();
        void StopLevel();

        void ResetLevel();

        void LookObject(const std::string& _geometryname,
                        const std::string& _objectname);

        void SetDynamicParam(bool _val);
        void SetFixedRotationParam(bool _val);
        void SetAngleParams(float _angle);

        inline bool IsChangedParams() const;
        inline void ResetChangedFlag();

        float GetAngleParams() const;

        inline bool IsObjectSelected() const;

        drash::SceneObjectParams GetSelectedParams() const;

    private:
        void SetProcessors();
        void SetCameraProcessors();
        void SetDragDropProcessors();

        bool InitCamera();
        bool InitPointLight();
        bool InitSpecialPoint();

        std::string fileNameLevel = "";

        LevelDesc* currentLevel = nullptr;
        greng::PointLight light1;
        greng::Camera* camera = nullptr;

        std::function<void()> treeRefreshHandler = []() {};
        std::function<std::string()> getSelectedTemplateHandler = []() {
            return std::string("");
        };
        bool playLevel = false;
        bool paused = false;
        bool changedParams = false;

        LevelObjectDesc* selectedObject = nullptr;

        MoveablePoint moveablePoint;
        RotationablePoint rotationablePoint;
        void RatateObject();
        void RenderPoints();

        Vec3f oldpositon;

        Timer timer;

        Vec2f camRotFirstClick;

        static const int MOVING_SPEED = 15;

        void RenderDragTemplate();
        bool dragNow = false;
        std::string dragTemplateName = "";

    private:
        LevelObjectDesc* SelectObject();
        void MoveOfAxis();
    };

    inline bool SceneEditorApp::IsSetFileName() const {
        return fileNameLevel != "";
    }

    inline bool SceneEditorApp::SaveLevel() {
        return SaveLevelAs(fileNameLevel);
    }

    inline LevelDesc* SceneEditorApp::GetCurrentLevel() const {
        return currentLevel;
    }

    inline std::string SceneEditorApp::GetLevelFileName() const {
        return fileNameLevel;
    }

    inline bool SceneEditorApp::IsSetLevel() const {
        return currentLevel != nullptr;
    }

    inline void
    SceneEditorApp::SetTreeRefreshHandler(const std::function<void()>& _han) {
        treeRefreshHandler = _han;
    }

    inline void SceneEditorApp::SetGetSelectedHandler(
        const std::function<std::string()>& _han) {
        getSelectedTemplateHandler = _han;
    }

    inline void SceneEditorApp::PauseLevel() {
        playLevel = false;
        paused = true;
    }

    inline bool SceneEditorApp::IsObjectSelected() const {
        return selectedObject != nullptr;
    }

    inline bool SceneEditorApp::IsChangedParams() const {
        return changedParams;
    }

    inline void SceneEditorApp::ResetChangedFlag() { changedParams = false; }
} // namespace drash
#endif // SCENEEDITORAPP_H
