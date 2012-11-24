#pragma once
#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "../sceneobjects.h"
#include "../cscene.h"
#include "../subsystem/explosionsystem.h"
#include "../subsystem/playerssystem.h"
#include "../subsystem/templatesystem.h"
#include "../subsystem/debugdrawsystem.h"

namespace drash
{

class CScene;

class CTestApp
{
public:
    virtual bool Init();
    virtual void Release();
    inline virtual void Step(double _dt);
    inline virtual void Render();

    inline CScene &GetScene();
    inline const CScene &GetScene() const;

    inline CPlayersSystem &GetPlayersSystem();
    inline CTemplateSystem &GetTemplateSystem();
    inline CDebugDrawSystem &GetDebugDrawSystem();

private:
    CScene mScene;
    CExplosionSystem mExplosionSystem;
    CPlayersSystem mPlayersSystem;
    CTemplateSystem mTemplateSystem;
    CDebugDrawSystem mDebugDrawSystem;
};

inline void CTestApp::Step(double _dt)
{
    mScene.Step(_dt);
}

inline void CTestApp::Render()
{
    mDebugDrawSystem.Draw();
}

inline CScene &CTestApp::GetScene()
{
    return mScene;
}

inline const CScene &CTestApp::GetScene() const
{
    return mScene;
}

inline CPlayersSystem &CTestApp::GetPlayersSystem()
{
    return mPlayersSystem;
}

inline CTemplateSystem &CTestApp::GetTemplateSystem()
{
    return mTemplateSystem;
}

inline CDebugDrawSystem &CTestApp::GetDebugDrawSystem()
{
    return mDebugDrawSystem;
}

} // namespace drash

#endif // CTESTAPP_H
