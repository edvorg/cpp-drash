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

} // namespace drash
