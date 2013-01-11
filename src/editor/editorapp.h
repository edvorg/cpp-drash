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

#ifndef CEDITORAPP_H
#define CEDITORAPP_H

#include "../app/app.h"
#include "../misc/moveablepoint.h"
#include "../diag/timer.h"
namespace drash {

enum State {
    BuildState,
    MoveState,
    MoveOfAxisState,
    StretchState,
    Simple
};

class CObjectEditorApp : public CApp
{
public:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    inline virtual void Render() override;
    virtual void Release() override;
    void StartBuild();

    inline bool IsStartBuild()const;

    bool BuildFigure(const std::string &_objectName);

    void AddNewObjectToTemplate(const std::string &_name);

    void ShowObject(const std::string &_name);

    inline void SetCurrentTemplateName(const std::string & _name);

    inline void SetTreeRefreshHandler(const std::function<void ()> &_han);

    inline void ActiveMoveMode();

    void ActiveStretchMode();

    void ActiveMoveOfAxisMode();

    void SaveCurrentObject();

    inline greng::CCamera *GetCamera();

//    SceneWidget *mSceneWidget = nullptr;
private:
    float GetCurDepth();

    void SetProcessors();

    void SetCameraProcessors();

    bool ValidateFigure();

    void RemoveCurrentObject();

    CFigure * SelectFigure(const CVec2f &_pos);

    void MoveFigure();

    void StretchFigure();

    void ChangeMode();

    void SelectVertex();

    void SettingCenterFigure();

    void MoveOfAxis();

private:
    CSceneObject *mCurrentObject = nullptr;

    State mState = Simple;

    std::string mCurrentTemplateName = "";

    std::function<void ()> mTreeRefreshHandler = [] () {};

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
};

inline bool CObjectEditorApp::IsStartBuild()const {
    return mState == BuildState;
}

inline void CObjectEditorApp::ActiveMoveMode() {
    mState = MoveState;
    ChangeMode();
}

inline void CObjectEditorApp::ActiveMoveOfAxisMode() {
    mState = MoveOfAxisState;
    ChangeMode();
}

inline void CObjectEditorApp::SetCurrentTemplateName(const std::string &_name){
    mCurrentTemplateName = _name;
}

inline void CObjectEditorApp::SetTreeRefreshHandler(const std::function<void ()> &_han) {
    mTreeRefreshHandler = _han;
}

inline greng::CCamera *CObjectEditorApp::GetCamera()
{
    return mCamera;
}

}// namespace drash
#endif // CEDITORAPP_H
