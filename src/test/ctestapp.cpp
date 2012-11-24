#include "ctestapp.h"

namespace drash
{

CTestApp::CTestApp()
{
}

CTestApp::~CTestApp()
{
}

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

    CCameraParams p;
    CCamera *cam = GetDebugDrawSystem().CreateCam(p);
    GetDebugDrawSystem().SetActiveCam(cam);

    if (cam == nullptr)
    {
        return false;
    }

    mTimer.Reset(true);

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

void CTestApp::Update()
{
    mTimer.Tick();
    mScene.Step( mTimer.GetDeltaTime() );
}

void CTestApp::Render()
{
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
