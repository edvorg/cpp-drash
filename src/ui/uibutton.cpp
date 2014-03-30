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

#include "uibutton.h"

#include "uisystem.h"
#include "../greng/renderer.h"
#include "../misc/math.h"

namespace drash {

    namespace ui {

        void UIButton::Connect(UISystem* _system) {
            UIWidget::Connect(_system);

            SetStepHandler([this](double _dt) {
                int local_x = GetUISystem()->GetCursorPosX() - GetPos().x;
                int local_y = GetUISystem()->GetCursorPosY() - GetPos().y;

                if (0 <= local_x && local_x <= static_cast<int>(GetSize().x) &&
                    0 <= local_y && local_y <= static_cast<int>(GetSize().y)) {
                    time += _dt * 2.0f;
                } else {
                    time -= _dt * 2.0f;
                }

                time = drash::math::Clamp<float>(time, 0, 1);
            });

            SetDrawHandler([this]() {
                Color4f c(pressed ? 0 : time, pressed ? 0.0f : 1.0f - time,
                          pressed ? 1 : 0, 1);
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
                // GetUISystem()->GetRenderer().DrawTriangle(p4, p2, p1, c);
                // GetUISystem()->GetRenderer().DrawTriangle(p3, p2, p4, c);

                // GetUISystem()->GetRenderer().DrawLine(p1, p2, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p3, p2, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p3, p4, 1, c);
                // GetUISystem()->GetRenderer().DrawLine(p1, p4, 1, c);
            });

            SetPressHandler([this]() { pressed = true; });

            SetReleaseHandler([this]() {
                if (pressed == true) {
                    pressed = false;

                    int local_x = GetUISystem()->GetCursorPosX() - GetPos().x;
                    int local_y = GetUISystem()->GetCursorPosY() - GetPos().y;

                    if (0 <= local_x &&
                        local_x <= static_cast<int>(GetSize().x) &&
                        0 <= local_y &&
                        local_y <= static_cast<int>(GetSize().y)) {
                        clickHandler();
                    }
                }
            });
        }

        void UIButton::SetClickHandler(const std::function<void()>& _handler) {
            clickHandler = _handler;
        }

    } // namepsace ui

} // namespace drash
