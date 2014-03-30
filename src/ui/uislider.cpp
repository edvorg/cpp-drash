// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "uislider.h"

#include "uisystem.h"
#include "../greng/renderer.h"
#include "../misc/math.h"

namespace drash {

    namespace ui {

        using namespace drash::math;

        void UISlider::Connect(UISystem* _system) {
            UIWidget::Connect(_system);

            SetPressHandler([this]() { pressed = true; });

            SetReleaseHandler([this]() { pressed = false; });

            SetStepHandler([this](double) {
                if (pressed == true) {
                    int hw = sliderWidth / 2;
                    int local_x = GetUISystem()->GetCursorPosX() - GetPos().x;
                    local_x = drash::math::Clamp<int>(local_x, hw,
                                                      (int)GetSize().x - hw) -
                              hw;
                    SetPercent((float)local_x /
                               ((float)GetSize().x - (float)sliderWidth));
                    valueHandler(value);
                }
            });

            SetDrawHandler([this]() {
                Color4f c(1, 0, 0, 1);

                Vec2f p1;
                Vec2f p2;
                Vec2f p3;
                Vec2f p4;
                GetUISystem()->UISpaceToScreenSpace(GetPos().x, GetPos().y, p1);
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x, GetPos().y + GetSize().y, p2);
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x + GetSize().x, GetPos().y + GetSize().y, p3);
                GetUISystem()->UISpaceToScreenSpace(GetPos().x + GetSize().x,
                                                    GetPos().y, p4);
                // FIXME 
                // GetUISystem()->GetRenderer().DrawLine(p1, p2, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p3, p2, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p3, p4, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p1, p4, 1, c);

                const unsigned int l = GetSize().x - sliderWidth;

                Vec2f p5;
                Vec2f p6;
                Vec2f p7;
                Vec2f p8;
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x + l * GetPercent(), GetPos().y, p5);
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x + l * GetPercent() + sliderWidth, GetPos().y,
                    p6);
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x + l * GetPercent() + sliderWidth,
                    GetPos().y + GetSize().y, p7);
                GetUISystem()->UISpaceToScreenSpace(
                    GetPos().x + l * GetPercent(), GetPos().y + GetSize().y,
                    p8);
                // FIXME 
                // GetUISystem()->GetRenderer().DrawLine(p5, p6, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p7, p6, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p7, p8, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p5, p8, 1, c);
            });
        }

        void UISlider::SetValueHandler(
            const std::function<void(float _value)>& _handler) {
            valueHandler = _handler;
        }

        void UISlider::SetValue(float _value) {
            value = Clamp<float>(_value, min, max);
        }

        float UISlider::GetValue() const { return value; }

        void UISlider::SetPercent(float _percent) {
            value = min + Clamp<float>(_percent, 0, 1) * (max - min);
        }

        float UISlider::GetPercent() const {
            return (value - min) / (max - min);
        }

        void UISlider::SetSliderWidth(unsigned int _width) {
            sliderWidth = Min(_width, GetSize().x / 2);
        }

        unsigned int UISlider::GetSliderWidth() const { return sliderWidth; }

        void UISlider::SetMin(float _min) {
            min = Min(_min, max);
            SetValue(value);
        }

        float UISlider::GetMin() const { return min; }

        void UISlider::SetMax(float _max) {
            max = Max(_max, min);
            SetValue(value);
        }

        float UISlider::GetMax() const { return max; }

    } // namepsace ui

} // namespace drash
