#include "subsystem.h"

namespace drash
{

CSubsystem::~CSubsystem()
{
    if (mScene != NULL)
    {
        mScene->DisconnectSubsystem(this);
        mScene = NULL;
    }
}

void CSubsystem::SetScene(CScene *_scene)
{
    if (mScene != NULL)
    {
        mScene->DisconnectSubsystem(this);
        mScene = NULL;
    }

    if (_scene == NULL)
    {
        return;
    }

    mScene = _scene;
    mScene->ConnectSubsystem(this);
}

}// namespace drash
