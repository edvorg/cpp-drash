#include "uislider.h"

#include "uisystem.h"
#include "../debugdrawsystem/debugdrawsystem.h"
#include "../misc/math.h"

namespace drash
{

namespace ui
{

using namespace drash::math;

void CUISlider::Connect(CUISystem *_system)
{
    CUIWidget::Connect(_system);

    SetPressHandler([this] ()
    {
        mPressed = true;
    });

    SetReleaseHandler([this] ()
    {
        mPressed = false;
    });

    SetStepHandler([this] (double)
    {
        if (mPressed == true)
        {
            int hw = mSliderWidth / 2;
            int local_x = GetUISystem()->GetCursorPosX() - GetX();
            local_x = drash::math::Clamp<int>(local_x, hw, (int)GetWidth() - hw) - hw;
            SetPercent((float)local_x / ((float)GetWidth() - (float)mSliderWidth));
            mValueHandler(mValue);
        }
    });

    SetDrawHandler([this] ()
    {
        b2Color c(1, 0, 0);

        CVec2f p1;
        CVec2f p2;
        CVec2f p3;
        CVec2f p4;
        GetUISystem()->UISpaceToScreenSpace(GetX(), GetY(), p1);
        GetUISystem()->UISpaceToScreenSpace(GetX(), GetY() + GetHeight(), p2);
        GetUISystem()->UISpaceToScreenSpace(GetX() + GetWidth(), GetY() + GetHeight(), p3);
        GetUISystem()->UISpaceToScreenSpace(GetX() + GetWidth(), GetY(), p4);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p4, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p4, c);

        const unsigned int l = GetWidth() - mSliderWidth;

        CVec2f p5;
        CVec2f p6;
        CVec2f p7;
        CVec2f p8;
        GetUISystem()->UISpaceToScreenSpace(GetX() + l * GetPercent(), GetY(), p5);
        GetUISystem()->UISpaceToScreenSpace(GetX() + l * GetPercent() + mSliderWidth, GetY(), p6);
        GetUISystem()->UISpaceToScreenSpace(GetX() + l * GetPercent() + mSliderWidth, GetY() + GetHeight(), p7);
        GetUISystem()->UISpaceToScreenSpace(GetX() + l * GetPercent(), GetY() + GetHeight(), p8);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p5, p6, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p7, p6, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p7, p8, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p5, p8, c);
    });
}

void CUISlider::SetValueHandler(const std::function<void (float _value)> &_handler)
{
    mValueHandler = _handler;
}

void CUISlider::SetValue(float _value)
{
    mValue = Clamp<float>(_value, mMin, mMax);
}

float CUISlider::GetValue() const
{
    return mValue;
}

void CUISlider::SetPercent(float _percent)
{
    mValue = mMin + Clamp<float>(_percent, 0, 1) * (mMax - mMin);
}

float CUISlider::GetPercent() const
{
    return (mValue - mMin) / (mMax - mMin);
}

void CUISlider::SetSliderWidth(unsigned int _width)
{
    mSliderWidth = Min(_width, GetWidth() / 2);
}

unsigned int CUISlider::GetSliderWidth() const
{
    return mSliderWidth;
}

void CUISlider::SetMin(float _min)
{
    mMin = Min(_min, mMax);
    SetValue(mValue);
}

float CUISlider::GetMin() const
{
    return mMin;
}

void CUISlider::SetMax(float _max)
{
    mMax = Max(_max, mMin);
    SetValue(mValue);
}

float CUISlider::GetMax() const
{
    return mMax;
}

} // namepsace ui

} // namespace drash
