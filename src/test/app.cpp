#include "app.h"

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

void CApp::PushEvent(const CAppEvent &_event)
{
    if (_event.GetType() != EventUnknown)
    {
        mEvents.push_back(_event);
    }
}

CAppEvent CApp::PopEvent()
{
    if (mEvents.size())
    {
        CAppEvent e(mEvents.back());
        mEvents.pop_back();
        return e;
    }
    else
    {
        return CAppEvent();
    }
}

} // namespace drash
