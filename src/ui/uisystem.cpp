#include "uisystem.h"

#include "uicontrol.h"

namespace drash
{

bool CUISystem::Init()
{
    return true;
}

void CUISystem::Release()
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        delete mControls[i];
        mControls[i] = nullptr;
    }
    mControlsCount = 0;
}

CUIControl *CUISystem::CreateControl()
{
    if (mControlsCount == mControlsCountLimit)
    {
        return nullptr;
    }

    return mControls[mControlsCount++] = new CUIControl;
}

} // namespace drash
