#pragma once
#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "../sceneobjects.h"
#include "../cscene.h"
#include "../subsystem/explosionsystem.h"
#include "../subsystem/playerssystem.h"
#include "../subsystem/templatesystem.h"

namespace drash
{

class CScene;

class CTestApp
{
public:
    CTestApp();
    virtual ~CTestApp();

    virtual bool Init();
    virtual void Release();
    virtual void Update();
    virtual void Render();

    CScene &GetScene();
    const CScene &GetScene() const;
    CCamera *GetCamera();

    CPlayersSystem &GetPlayersSystem();
    CTemplateSystem &GetTemplateSystem();

private:
    CScene mScene;
    CCamera *mCamera;
    CTimer mTimer;
    CExplosionSystem mExplosionSystem;
    CPlayersSystem mPlayersSystem;
    CTemplateSystem mTemplateSystem;
};

} // namespace drash

#endif // CTESTAPP_H
