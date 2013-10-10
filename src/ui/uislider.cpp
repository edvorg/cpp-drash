// DRASH_LICENSE_BEGIN
/*

  drash GPL Source Code
  Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

  This file is part of the drash GPL Source Code (drash Source Code).

  drash Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  drash Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "uislider.h"

#include "uisystem.h"
#include "../greng/renderer.h"
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
        GetUISystem()->GetRenderer()->DrawLine(p1, p2, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p3, p2, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p3, p4, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p1, p4, 1, c);

        const unsigned int l = GetSize().mX - mSliderWidth;

        CVec2f p5;
        CVec2f p6;
        CVec2f p7;
        CVec2f p8;
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent(), GetPos().mY, p5);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent() + mSliderWidth, GetPos().mY, p6);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent() + mSliderWidth, GetPos().mY + GetSize().mY, p7);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + l * GetPercent(), GetPos().mY + GetSize().mY, p8);
        GetUISystem()->GetRenderer()->DrawLine(p5, p6, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p7, p6, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p7, p8, 1, c);
        GetUISystem()->GetRenderer()->DrawLine(p5, p8, 1, c);
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
