#pragma once
#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "../sceneobjects.h"
#include "../cscene.h"
#include "../subsystem/explosionsystem.h"
#include "../subsystem/playerssystem.h"
#include "../subsystem/templatesystem.h"
#include "../subsystem/debugdrawsystem.h"
#include "appevent.h"

#include <list>

namespace drash
{

class CScene;

class CApp
{
public:
    virtual ~CApp() {}

    virtual bool Init();
    virtual void Release();
    inline virtual void Step(double _dt);
    inline virtual void Render();

    inline CScene &GetScene();
    inline const CScene &GetScene() const;

    inline CPlayersSystem &GetPlayersSystem();
    inline CTemplateSystem &GetTemplateSystem();
    inline CDebugDrawSystem &GetDebugDrawSystem();

    void PushEvent(const CAppEvent &_event);
    CAppEvent PopEvent();

protected:

private:
    CScene mScene;
    CExplosionSystem mExplosionSystem;
    CPlayersSystem mPlayersSystem;
    CTemplateSystem mTemplateSystem;
    CDebugDrawSystem mDebugDrawSystem;
    std::list<CAppEvent> mEvents;
};

inline void CApp::Step(double _dt)
{
    mScene.Step(_dt);
}

inline void CApp::Render()
{
    mDebugDrawSystem.Draw();
}

inline CScene &CApp::GetScene()
{
    return mScene;
}

inline const CScene &CApp::GetScene() const
{
    return mScene;
}

inline CPlayersSystem &CApp::GetPlayersSystem()
{
    return mPlayersSystem;
}

inline CTemplateSystem &CApp::GetTemplateSystem()
{
    return mTemplateSystem;
}

inline CDebugDrawSystem &CApp::GetDebugDrawSystem()
{
    return mDebugDrawSystem;
}

} // namespace drash

#endif // CTESTAPP_H
