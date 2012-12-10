#include "uisystem.h"

#include "uicontrol.h"
#include "../misc/math.h"
#include "../misc/cvec2.h"

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

void CUISystem::DestroyControl(CUIControl *_control)
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        if (mControls[i] == _control)
        {
            _control->mDestroyHandler();
            delete _control;
            mControls[i] = nullptr;
            mControlsCount--;
            return;
        }
    }
}

void CUISystem::SetAspectRatio(float _ratio)
{
    mAspectRatio = _ratio;
    SetWidth(mWidth);
}

void CUISystem::SetWidth(unsigned int _width)
{
    mWidth = _width;
    mHeight = mWidth / mAspectRatio;
}

bool CUISystem::ScreenSpaceToUISpace(const CVec2 &_from, unsigned int &_x, unsigned int &_y)
{
    _x = drash::math::Clamp(_from.x + 0.5f, 0.0f, 1.0f) * static_cast<float>(mWidth);
    _y = drash::math::Clamp(_from.y + 0.5f, 0.0f, 1.0f) * static_cast<float>(mHeight);
    return true;
}

bool CUISystem::UISpaceToScreenSpace(unsigned int _x, unsigned int _y, CVec2 &_v)
{
    _v.x = (static_cast<float>(drash::math::Clamp<unsigned int>(_x, 0, mWidth)) /
            static_cast<float>(mWidth)) - 0.5f;
    _v.y = (static_cast<float>(drash::math::Clamp<unsigned int>(_y, 0, mHeight)) /
            static_cast<float>(mHeight)) - 0.5f;
    return true;
}

} // namespace drash
