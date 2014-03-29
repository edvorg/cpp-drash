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

#include "app.h"
#include "../greng/camera.h"

namespace drash {

    CApp::CApp()
        : mGrengSystems(), mScene({}), mPlayersSystem(mScene),
          mGeometryManager(mScene), mEventSystem(),
          mUISystem(GetGrengSystems().GetRenderer()),
          mLevelManager(mScene, mGeometryManager),
          mDebugRenderer(mGrengSystems, mScene, mGeometryManager) {}

    void CApp::Step(double _dt) {
        if (mQuit) {
            mQuitHandler();
            return;
        }

        mCurrentTimeDelta = _dt;

        mGrengSystems.Step(_dt);
        mEventSystem.Process();
        mScene.Step(_dt);
        mPlayersSystem.Step(_dt);
        mGeometryManager.Step(_dt);
        mUISystem.Step(_dt);
    }

    void CApp::Render() {
        mDebugRenderer.Render();
        mUISystem.DebugDraw();
    }

} // namespace drash
