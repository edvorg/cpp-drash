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

#include "uiwidget.h"

#include "uisystem.h"
#include "uicontrol.h"

namespace drash {

    namespace ui {

        UIWidget::~UIWidget() { this->Disconnect(); }

        void UIWidget::Connect(UISystem* _system) {
            uISystem = _system;
            uIControl = uISystem->CreateControl();
            uIControl->SetDestroyHandler([this]() {
                uIControl = nullptr;
                uISystem = nullptr;
            });
        }

        void UIWidget::Disconnect() {
            if (uIControl != nullptr) {
                uISystem->DestroyControl(uIControl);
                uISystem = nullptr;
                uIControl = nullptr;
            }
        }

        UISystem* UIWidget::GetUISystem() const { return uISystem; }

        void UIWidget::SetPressHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetPressHandler(_handler);
            }
        }

        void
        UIWidget::SetReleaseHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetReleaseHandler(_handler);
            }
        }

        void
        UIWidget::SetStepHandler(const std::function<void(double)>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetStepHandler(_handler);
            }
        }

        void UIWidget::SetDrawHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetDrawHandler(_handler);
            }
        }

        void UIWidget::SetPos(const Vec2i& _pos) {
            if (uIControl != nullptr) {
                uIControl->SetPos(_pos);
            }
        }

        void UIWidget::SetSize(const Vec2ui& _size) {
            if (uIControl != nullptr) {
                uIControl->SetSize(_size);
            }
        }

        const Vec2i UIWidget::GetPos() const {
            if (uIControl != nullptr) {
                return uIControl->GetPos();
            } else {
                return Vec2i(0);
            }
        }

        const Vec2ui UIWidget::GetSize() const {
            if (uIControl != nullptr) {
                return uIControl->GetSize();
            } else {
                return Vec2ui(0);
            }
        }

    } // namepsace ui

} // namespace drash
