// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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
#include "../misc/moveablepoint.h"
#include "../diag/timer.h"

namespace drash {

class CSceneEditorApp : public CApp
{
public:
    CSceneEditorApp();

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;
    virtual void Release() override;

    void UpdateTemplateSystem();

    bool LoadLevel(const std::string &_filename);

    bool SaveLevelAs(const std::string &_filename);

    bool NewLevel();

    void StartCurrentLevel();

    void AddObject(const std::string &_name);

    //
    inline bool SaveLevel();

    inline bool IsSetFileName()const;
    inline bool IsSetLevel() const;
    inline CLevel * GetCurrentLevel() const;
    inline std::string GetLevelFileName() const;

    inline void SetTreeRefreshHandler(const std::function<void ()> & _han);
    inline void PauseLevel();
    void StopLevel();

    void ResetLevel();
private:
    void SetProcessors();
    void SetCameraProcessors();


    bool InitCamera();
    bool InitPointLight();

    std::string mFileNameLevel = "";

    CLevel * mCurrentLevel = nullptr;
    greng::CPointLight mLight1;
    greng::CCamera *mCamera = nullptr;

    std::function<void ()> mTreeRefreshHandler = [] () {};

    bool mPlayLevel = false;

    CSceneObject * mSelectedObject = nullptr;

    //CSceneObjectParams * mSelectedObject = nullptr;

    CMoveablePoint mMoveablePoint;

    CVec3f mOldpositon;

    CTimer mTimer;

    CVec2f mCamRotFirstClick;

    static const int MOVING_SPEED = 15;

    std::map<CSceneObject*, CSceneObjectParams*> mObjectParams;

    void StoreParams();

private:
    CSceneObject * SelectObject();
    void MoveOfAxis();
};

inline bool CSceneEditorApp::IsSetFileName()const {
    return mFileNameLevel != "";
}

inline bool CSceneEditorApp::SaveLevel() {
    return SaveLevelAs(mFileNameLevel);
}

inline CLevel * CSceneEditorApp::GetCurrentLevel() const {
    return mCurrentLevel;
}

inline std::string CSceneEditorApp::GetLevelFileName() const {
    return mFileNameLevel;
}

inline bool CSceneEditorApp::IsSetLevel() const {
    return mCurrentLevel != nullptr;
}

inline void CSceneEditorApp::SetTreeRefreshHandler(const std::function<void ()> &_han) {
    mTreeRefreshHandler = _han;
}

inline void CSceneEditorApp::PauseLevel() {
    mPlayLevel = false;
}


} // namespace drash
#endif // SCENEEDITORAPP_H
