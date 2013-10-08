#include "screen.h"

namespace drash
{

CScreen::CScreen(CRoot & _parent):
    mRoot(_parent)
{
    mUISystem.Init();
    mEventSystem.Init();
}

CScreen::~CScreen()
{
    mUISystem.Release();
    mEventSystem.Release();
}

void CScreen::Step(double _dt)
{
    GetUISystem().Step(_dt);
}

void CScreen::Render()
{
    GetUISystem().Render();
}

} // namespace drash
