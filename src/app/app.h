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

#pragma once
#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "../players/playerssystem.h"
#include "../scene/scene.h"
#include "../scene/geometrymanager.h"
#include "../debugrenderer/debugrenderer.h"
#include "appeventsystem.h"
#include "../ui/uisystem.h"
#include "../greng/grengsystemsset.h"
#include "../levelmanager/levelmanager.h"

namespace drash {

    class CApp {
    public:
        CApp();
        virtual ~CApp() = default;

        virtual void Step(double _dt);
        virtual void Render();

        inline void Quit() { quit = true; }
        inline void SetQuitHandler(std::function<void()> _handler) {
            quitHandler = _handler;
        }

        auto& GetScene() { return scene; }
        auto& GetEventSystem() { return eventSystem; }
        auto& GetPlayersSystem() { return playersSystem; }
        auto& GetGeometryManager() { return geometryManager; }
        auto& GetDebugRenderer() { return debugRenderer; }
        auto& GetUISystem() { return uISystem; }
        auto& GetLevelManager() { return levelManager; }
        auto& GetGrengSystems() { return grengSystems; }

        /// used to make CApp childs about mouse moving event
        /// use this from your CApp back end (Qt, SDL, etc.)
        /// we assume that _pos is coordinates in screen space (-0.5, -0.5)
        /// (0.5, 0.5)
        void SetCursorPos(const CVec2f& _pos) { cursorPos = _pos; }

        /// used by CApp childs for detection, where mouse cursor is
        /// returns coordinates in screen space (-0.5, -0.5) (0.5, 0.5)
        auto& GetCursorPos() const { return cursorPos; }

        auto& GetCurrentTimeDelta() const { return currentTimeDelta; }

    protected:
    private:
        CVec2f cursorPos = CVec2f(0);

        greng::CGrengSystemsSet grengSystems;
        CScene scene;
        CPlayersSystem playersSystem;
        CGeometryManager geometryManager;
        CAppEventSystem eventSystem;
        ui::CUISystem uISystem;
        CLevelManager levelManager;
        CDebugRenderer debugRenderer;

        std::function<void()> quitHandler = []() {};
        bool quit = false;
        double currentTimeDelta = 0;
    };

} // namespace drash

#endif // CTESTAPP_H
