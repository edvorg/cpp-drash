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

namespace drash
{

CApp::CApp()
{
}

bool CApp::Init()
{
    CSceneParams params;
    params.mGravity.Set( 0, -9.8 );

	mQuit = true;

    if ( mScene.Init(params) == false )
    {
        return false;
    }

    mExplosionSystem.SetScene(&mScene);
    mPlayersSystem.SetScene(&mScene);
    mTemplateSystem.SetScene(&mScene);
    mDebugDrawSystem.SetScene(&mScene);

    CCamera *cam = GetDebugDrawSystem().CreateCam(CCameraParams(), true);

    if (cam == nullptr)
    {
        return false;
    }

    mQuit = false;

    return true;
}

void CApp::Release()
{
    mPlayersSystem.Release();
    mExplosionSystem.Release();
    mTemplateSystem.Release();
    mDebugDrawSystem.Release();

    mPlayersSystem.SetScene(nullptr);
    mExplosionSystem.SetScene(nullptr);
    mTemplateSystem.SetScene(nullptr);
    mDebugDrawSystem.SetScene(nullptr);

    mScene.Release();
}

void CApp::Step(double _dt)
{
    if (mQuit)
    {
        this->Release();
        mQuitHandler();
        return;
    }

    mEventSystem.Process();

    mScene.Step(_dt);
}

void CApp::Render()
{
    mDebugDrawSystem.Draw();
}

} // namespace drash
