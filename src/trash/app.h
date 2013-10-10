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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#pragma once
#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "../explosion/explosionsystem.h"
#include "../players/playerssystem.h"
#include "../scene/scene.h"
#include "../scene/geometrymanager.h"
#include "../debugrenderer/debugrenderer.h"
#include "appeventsystem.h"
#include "../ui/uisystem.h"
#include "../greng/grengsystemsset.h"
#include "../levelmanager/levelmanager.h"

namespace drash
{

class CApp
{
public:
    CApp();
    virtual ~CApp() { }

    virtual bool Init();
    virtual void Release();
    virtual void Step(double _dt);
    virtual void Render();

    inline void Quit();
    inline void SetQuitHandler(std::function<void ()> _handler);

    inline CScene &GetScene();
    inline CAppEventSystem &GetEventSystem();
	inline CExplosionSystem &GetExplosionSystem();
    inline CPlayersSystem &GetPlayersSystem();
    inline CGeometryManager &GetGeometryManager();
    inline CDebugRenderer &GetDebugRenderer();
    inline ui::CUISystem &GetUISystem();
    inline CLevelManager &GetLevelManager();
    inline greng::CGrengSystemsSet &GetGrengSystems();

    /// used to make CApp childs about mouse moving event
    /// use this from your CApp back end (Qt, SDL, etc.)
    /// we assume that _pos is coordinates in screen space (-0.5, -0.5) (0.5, 0.5)
    inline void SetCursorPos(const CVec2f &_pos);

    /// used by CApp childs for detection, where mouse cursor is
    /// returns coordinates in screen space (-0.5, -0.5) (0.5, 0.5)
    inline const CVec2f &GetCursorPos() const;

    inline double GetCurrentTimeDelta() const;

protected:

private:
    CAppEventSystem mEventSystem;
    CVec2f mCursorPos = CVec2f(0);

    CScene mScene;
    CExplosionSystem mExplosionSystem;
    CPlayersSystem mPlayersSystem;
    CGeometryManager mGeometryManager;
    CDebugRenderer mDebugRenderer;
    ui::CUISystem mUISystem;
    CLevelManager mLevelManager;
    greng::CGrengSystemsSet mGrengSystems;

    std::function<void ()> mQuitHandler = [] () {};
    bool mQuit = false;
    double mCurrentTimeDelta = 0;
};

inline void CApp::Quit()
{
    mQuit = true;
}

inline void CApp::SetQuitHandler(std::function<void ()> _handler)
{
    mQuitHandler = _handler;
}

inline CScene &CApp::GetScene()
{
    return mScene;
}

inline CAppEventSystem &CApp::GetEventSystem()
{
    return mEventSystem;
}

inline CExplosionSystem &CApp::GetExplosionSystem()
{
    return mExplosionSystem;
}

inline CPlayersSystem &CApp::GetPlayersSystem()
{
    return mPlayersSystem;
}

inline CGeometryManager &CApp::GetGeometryManager()
{
    return mGeometryManager;
}

inline CDebugRenderer &CApp::GetDebugRenderer()
{
    return mDebugRenderer;
}

inline ui::CUISystem &CApp::GetUISystem()
{
    return mUISystem;
}

inline CLevelManager &CApp::GetLevelManager()
{
    return mLevelManager;
}

inline greng::CGrengSystemsSet &CApp::GetGrengSystems()
{
    return mGrengSystems;
}

inline void CApp::SetCursorPos(const CVec2f &_pos)
{
    mCursorPos = _pos;
}

inline const CVec2f &CApp::GetCursorPos() const
{
    return mCursorPos;
}

inline double CApp::GetCurrentTimeDelta() const
{
    return mCurrentTimeDelta;
}

} // namespace drash

#endif // CTESTAPP_H
