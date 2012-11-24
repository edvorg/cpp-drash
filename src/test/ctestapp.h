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
    virtual void Step(double _dt);
    virtual void Render();

    CScene &GetScene();
    const CScene &GetScene() const;

    CPlayersSystem &GetPlayersSystem();
    CTemplateSystem &GetTemplateSystem();
    CDebugDrawSystem &GetDebugDrawSystem();

private:
    CScene mScene;
    CExplosionSystem mExplosionSystem;
    CPlayersSystem mPlayersSystem;
    CTemplateSystem mTemplateSystem;
    CDebugDrawSystem mDebugDrawSystem;
};

} // namespace drash

#endif // CTESTAPP_H
