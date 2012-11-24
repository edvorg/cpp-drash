#include "ctestapp.h"

namespace drash
{

bool CTestApp::Init()
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

void CTestApp::Release()
{
    mPlayersSystem.SetScene(nullptr);
    mExplosionSystem.SetScene(nullptr);
    mTemplateSystem.SetScene(nullptr);
    mDebugDrawSystem.SetScene(nullptr);

    mScene.Release();
}

void CTestApp::Step(double _dt)
{
    mScene.Step(_dt);
}

void CTestApp::Render()
{
    mDebugDrawSystem.Draw();
}

CScene &CTestApp::GetScene()
{
    return mScene;
}

const CScene &CTestApp::GetScene() const
{
    return mScene;
}

CPlayersSystem &CTestApp::GetPlayersSystem()
{
    return mPlayersSystem;
}

CTemplateSystem &CTestApp::GetTemplateSystem()
{
    return mTemplateSystem;
}

CDebugDrawSystem &CTestApp::GetDebugDrawSystem()
{
    return mDebugDrawSystem;
}

} // namespace drash
