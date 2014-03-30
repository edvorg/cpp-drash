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

    App::App(greng::GrengSystemsSet& greng)
        : grengSystems(greng), scene({}), playersSystem(scene),
          geometryManager(scene), eventSystem(),
          uISystem(GetGrengSystems().GetRenderer()),
          levelManager(scene, geometryManager),
          debugRenderer(grengSystems, scene, geometryManager) {}

    void App::Step(double _dt) {
        if (quit) {
            quitHandler();
            return;
        }

        currentTimeDelta = _dt;

        grengSystems.Step(_dt);
        eventSystem.Process();
        scene.Step(_dt);
        playersSystem.Step(_dt);
        geometryManager.Step(_dt);
        uISystem.Step(_dt);
    }

    void App::Render() {
        debugRenderer.Render();
        uISystem.DebugDraw();
    }

} // namespace drash
