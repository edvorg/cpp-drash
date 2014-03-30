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

#include "uisystem.h"

#include "uicontrol.h"

namespace drash {

    namespace ui {

        UISystem::UISystem(greng::Renderer& _renderer) : renderer(_renderer) {}

        UISystem::~UISystem() {
            for (unsigned int i = 0; i < controlsCount; i++) {
                delete controls[i];
                controls[i] = nullptr;
            }
            controlsCount = 0;
        }

        UIControl* UISystem::CreateControl() {
            if (controlsCount == controlsCountLimit) {
                return nullptr;
            }

            return controls[controlsCount++] = new UIControl;
        }

        void UISystem::DestroyControl(UIControl* _control) {
            for (unsigned int i = 0; i < controlsCount; i++) {
                if (controls[i] == _control) {
                    _control->destroyHandler();
                    delete _control;
                    controls[i] = nullptr;
                    controlsCount--;
                    return;
                }
            }
        }

        void UISystem::SetAspectRatio(float _ratio) {
            aspectRatio = _ratio;
            SetWidth(width);
        }

        void UISystem::SetWidth(unsigned int _width) {
            width = _width;
            height = width / aspectRatio;
        }

        bool UISystem::ScreenSpaceToUISpace(const Vec2f& _from, int& _x,
                                            int& _y) {
            _x = (_from.x + 0.5f) * static_cast<float>(width);
            _y = (_from.y + 0.5f) * static_cast<float>(height);
            return true;
        }

        bool UISystem::UISpaceToScreenSpace(int _x, int _y, Vec2f& _v) {
            _v.x = (static_cast<float>(_x) / static_cast<float>(width)) - 0.5f;
            _v.y = (static_cast<float>(_y) / static_cast<float>(height)) - 0.5f;
            return true;
        }

        void UISystem::SetCursorPos(int _x, int _y) {
            cursorX = _x;
            cursorY = _y;
        }

        void UISystem::BeginEvent() {
            for (unsigned int i = 0; i < controlsCount; i++) {
                int local_x = GetCursorPosX() - controls[i]->GetPos().x;
                int local_y = GetCursorPosY() - controls[i]->GetPos().y;

                if (0 <= local_x &&
                    local_x <= static_cast<int>(controls[i]->GetSize().x) &&
                    0 <= local_y &&
                    local_y <= static_cast<int>(controls[i]->GetSize().y)) {
                    pressedControl = controls[i];
                    pressedControl->pressHandler();
                    break;
                }
            }
        }

        void UISystem::EndEvent() {
            if (pressedControl != nullptr) {
                pressedControl->releaseHandler();
                pressedControl = nullptr;
            }
        }

        void UISystem::Step(double _dt) {
            for (unsigned int i = 0; i < controlsCount; i++) {
                controls[i]->stepHandler(_dt);
            }
        }

        void UISystem::DebugDraw() const {
            for (unsigned int i = 0; i < controlsCount; i++) {
                controls[i]->drawHandler();
            }
        }

    } // namepsace ui

} // namespace drash
