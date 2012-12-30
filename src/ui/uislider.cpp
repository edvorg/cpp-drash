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
            int local_x = GetUISystem()->GetCursorPosX() - GetPos().mX;
            local_x = drash::math::Clamp<int>(local_x, hw, (int)GetSize().mX - hw) - hw;
            SetPercent((float)local_x / ((float)GetSize().mX - (float)mSliderWidth));
            mValueHandler(mValue);
        }
    });

    SetDrawHandler([this] ()
    {
        CColor4f c(1, 0, 0, 1);

        CVec2f p1;
        CVec2f p2;
        CVec2f p3;
        CVec2f p4;
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX, GetPos().mY, p1);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX, GetPos().mY + GetSize().mY, p2);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + GetSize().mX, GetPos().mY + GetSize().mY, p3);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + GetSize().mX, GetPos().mY, p4);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p4, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p4, c);

        const unsigned int l = GetSize().mX - mSliderWidth;

        CVec2f p5;
        CVec2f p6;
        CVec2f p7;
        CVec2f p8;
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent(), GetPos().mY, p5);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent() + mSliderWidth, GetPos().mY, p6);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent() + mSliderWidth, GetPos().mY + GetSize().mY, p7);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent(), GetPos().mY + GetSize().mY, p8);
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
    mSliderWidth = Min(_width, GetSize().mX / 2);
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
