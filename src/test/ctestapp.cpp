#include "ctestapp.h"

namespace drash
{

bool CApp::Init()
{
    CSceneParams params;
    params.mGravity.Set( 0, -9.8 );

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

    return true;
}

void CApp::Release()
{
    mPlayersSystem.SetScene(nullptr);
    mExplosionSystem.SetScene(nullptr);
    mTemplateSystem.SetScene(nullptr);
    mDebugDrawSystem.SetScene(nullptr);

    mScene.Release();
}

} // namespace drash
