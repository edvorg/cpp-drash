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

#include "sceneeditorapp.h"
#include "../scene/sceneobject.h"
#include "../levelmanager/level.h"

namespace drash {

CSceneEditorApp::CSceneEditorApp()
{
}

bool CSceneEditorApp::Init()
{
    CApp::Init();
    if (InitCamera() == false ||
        InitPointLight() == false) {
        return false;
    }
    return true;
    //GetTemplateSystem().CreateSceneObjectFromTemplate("Object1",CSceneObjectParams());
}

void CSceneEditorApp::Step(double _dt)
{
    if (mPlayLevel == true ){
        CApp::Step(_dt);
    } else {
        CApp::Step(0);
    }
}

void CSceneEditorApp::Render()
{
    CApp::Render();
}

void CSceneEditorApp::Release()
{
    CApp::Release();
}

void CSceneEditorApp::UpdateTemplateSystem()
{
    GetTemplateSystem().Load();
}

bool CSceneEditorApp::LoadLevel(const std::string &_filename)
{
    if (mCurrentLevel != nullptr) {
        GetLevelManager().DestroyLevel(mCurrentLevel);
        mCurrentLevel = nullptr;
    }

    mCurrentLevel = GetLevelManager().CreateLevel();

    if (mCurrentLevel == nullptr) {
        return false;
    }

    if (mCurrentLevel->Load(_filename) == false) {
        return false;
    }

    mFileNameLevel = _filename;

    mTreeRefreshHandler();
    GetLevelManager().StartLevel(mCurrentLevel);
    mPlayLevel = false;
    return true;
    //GetLevelManager().StartLevel(level);
}

bool CSceneEditorApp::SaveLevelAs(const std::string &_filename)
{
    if (mCurrentLevel == nullptr) {
        return false;
    }

    return mCurrentLevel->Store(_filename);
}

void CSceneEditorApp::StartCurrentLevel()
{
    if (mCurrentLevel != nullptr) {
        mPlayLevel = true;
        //GetLevelManager().StartLevel(mCurrentLevel);
    }
}

void CSceneEditorApp::SetProcessors()
{
}

bool CSceneEditorApp::InitCamera()
{
    greng::CCameraParams p;
    p.mPos.Set(10,10,10.0f);
    p.mRotation.Set(-M_PI/4, M_PI/4, 0);
    mCamera = GetCameraManager().CreateCamera(p);
    GetDebugRenderer().SetCamera(mCamera);

    return true;
}

bool CSceneEditorApp::InitPointLight()
{
    mLight1.mPosition.Set(0, 10, 0);
    GetDebugRenderer().SetLight(&mLight1);
    return true;
}

} // namspace drash
