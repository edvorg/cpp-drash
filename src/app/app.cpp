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

#include "app.h"
#include "../greng/camera.h"

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

    if (mGrengSystems.Init() == false)
    {
        return false;
    }

    if (mScene.Init(params) == false)
    {
        return false;
    }

    mExplosionSystem.SetScene(&mScene);
    mPlayersSystem.SetScene(&mScene);
    mTemplateSystem.SetScene(&mScene);
    mUISystem.SetRenderer(&GetGrengSystems().GetRenderer());
    mLevelManager.SetScene(&mScene);
    mLevelManager.SetTemplateSystem(&mTemplateSystem);

    if (mExplosionSystem.Init() == false ||
        mPlayersSystem.Init() == false ||
        mTemplateSystem.Init() == false ||
        mEventSystem.Init() == false ||
        mUISystem.Init() == false ||
        mLevelManager.Init() == false)
    {
        return false;
    }

    mDebugRenderer.SetGrengSystems(&mGrengSystems);
    mDebugRenderer.SetScene(&mScene);

    if (mDebugRenderer.Init() == false)
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

    mGrengSystems.Step(_dt);
    mEventSystem.Process();
    mScene.Step(_dt);
    mExplosionSystem.Step(_dt);
    mPlayersSystem.Step(_dt);
    mTemplateSystem.Step(_dt);
    mUISystem.Step(_dt);
}

void CApp::Release()
{
    mLevelManager.Release();
    mDebugRenderer.Release();
    mEventSystem.Release();
    mExplosionSystem.Release();
    mPlayersSystem.Release();
    mTemplateSystem.Release();
    mUISystem.Release();
    mScene.Release();
    mGrengSystems.Release();
}

void CApp::Render()
{
    mDebugRenderer.Render();
    mUISystem.DebugDraw();
}

} // namespace drash
