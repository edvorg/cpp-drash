#include "uisystem.h"

#include "uicontrol.h"
#include "../misc/vec2.h"

namespace drash
{

namespace ui
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

bool CUISystem::ScreenSpaceToUISpace(const CVec2f &_from, int &_x, int &_y)
{
    _x = (_from.mX + 0.5f) * static_cast<float>(mWidth);
    _y = (_from.mY + 0.5f) * static_cast<float>(mHeight);
    return true;
}

bool CUISystem::UISpaceToScreenSpace(int _x, int _y, CVec2f &_v)
{
    _v.mX = (static_cast<float>(_x) / static_cast<float>(mWidth)) - 0.5f;
    _v.mY = (static_cast<float>(_y) / static_cast<float>(mHeight)) - 0.5f;
    return true;
}

void CUISystem::SetCursorPos(int _x, int _y)
{
    mCursorX = _x;
    mCursorY = _y;
}

void CUISystem::BeginEvent()
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        int local_x = GetCursorPosX() - mControls[i]->GetX();
        int local_y = GetCursorPosY() - mControls[i]->GetY();

        if (0 <= local_x && local_x <= static_cast<int>(mControls[i]->GetWidth()) &&
            0 <= local_y && local_y <= static_cast<int>(mControls[i]->GetHeight()))
        {
            mPressedControl = mControls[i];
            mPressedControl->mPressHandler();
            break;
        }
    }
}

void CUISystem::EndEvent()
{
    if (mPressedControl != nullptr)
    {
        mPressedControl->mReleaseHandler();
        mPressedControl = nullptr;
    }
}

void CUISystem::SetDebugDrawSystem(CDebugDrawSystem *_system)
{
    mDebugDrawSystem = _system;
}

void CUISystem::Step(double _dt)
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        mControls[i]->mStepHandler(_dt);
    }
}

void CUISystem::DebugDraw() const
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        mControls[i]->mDrawHandler();
    }
}

} // namepsace ui

} // namespace drash
