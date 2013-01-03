// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "app.h"
#include "../debugdrawsystem/camera.h"

namespace drash
{

CApp::CApp()
{
}

bool CApp::Init()
{
    mQuit = true;

    CSceneParams params;
    params.mGravity.Set( 0, -9.8 );

    if (mScene.Init(params) == false ||
        mExplosionSystem.Init() == false ||
        mPlayersSystem.Init() == false ||
        mTemplateSystem.Init() == false ||
        mDebugDrawSystem.Init() == false ||
        mEventSystem.Init() == false ||
        mUISystem.Init() == false ||
        mRenderer.Init() == false)
    {
        return false;
    }

    mExplosionSystem.SetScene(&mScene);
    mPlayersSystem.SetScene(&mScene);
    mTemplateSystem.SetScene(&mScene);
    mDebugDrawSystem.SetScene(&mScene);
    mUISystem.SetDebugDrawSystem(&mDebugDrawSystem);

    CCamera *cam = GetDebugDrawSystem().CreateCam(CCameraParams(), true);

    if (cam == nullptr)
    {
        return false;
    }

    mQuit = false;

    return true;
}

void CApp::Step(double _dt)
{
    if (mQuit)
    {
        mQuitHandler();
        return;    
    }

    mCurrentTimeDelta = _dt;

    mEventSystem.Process();
    mScene.Step(_dt);
    mExplosionSystem.Step(_dt);
    mPlayersSystem.Step(_dt);
    mTemplateSystem.Step(_dt);
    mDebugDrawSystem.Step(_dt);
    mUISystem.Step(_dt);
}

void CApp::Release()
{
    mExplosionSystem.SetScene(nullptr);
    mPlayersSystem.SetScene(nullptr);
    mTemplateSystem.SetScene(nullptr);
    mDebugDrawSystem.SetScene(nullptr);
    mUISystem.SetDebugDrawSystem(nullptr);

    mEventSystem.Release();
    mScene.Release();
    mExplosionSystem.Release();
    mPlayersSystem.Release();
    mTemplateSystem.Release();
    mDebugDrawSystem.Release();
    mUISystem.Release();
}

void CApp::Render()
{
    mDebugDrawSystem.Draw();
    mUISystem.DebugDraw();
}

} // namespace drash
