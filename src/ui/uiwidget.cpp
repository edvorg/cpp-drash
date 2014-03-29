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

        CUIWidget::~CUIWidget() { this->Disconnect(); }

        void CUIWidget::Connect(CUISystem* _system) {
            uISystem = _system;
            uIControl = uISystem->CreateControl();
            uIControl->SetDestroyHandler([this]() {
                uIControl = nullptr;
                uISystem = nullptr;
            });
        }

        void CUIWidget::Disconnect() {
            if (uIControl != nullptr) {
                uISystem->DestroyControl(uIControl);
                uISystem = nullptr;
                uIControl = nullptr;
            }
        }

        CUISystem* CUIWidget::GetUISystem() const { return uISystem; }

        void CUIWidget::SetPressHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetPressHandler(_handler);
            }
        }

        void
        CUIWidget::SetReleaseHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetReleaseHandler(_handler);
            }
        }

        void
        CUIWidget::SetStepHandler(const std::function<void(double)>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetStepHandler(_handler);
            }
        }

        void CUIWidget::SetDrawHandler(const std::function<void()>& _handler) {
            if (uIControl != nullptr) {
                uIControl->SetDrawHandler(_handler);
            }
        }

        void CUIWidget::SetPos(const CVec2i& _pos) {
            if (uIControl != nullptr) {
                uIControl->SetPos(_pos);
            }
        }

        void CUIWidget::SetSize(const CVec2ui& _size) {
            if (uIControl != nullptr) {
                uIControl->SetSize(_size);
            }
        }

        const CVec2i CUIWidget::GetPos() const {
            if (uIControl != nullptr) {
                return uIControl->GetPos();
            } else {
                return CVec2i(0);
            }
        }

        const CVec2ui CUIWidget::GetSize() const {
            if (uIControl != nullptr) {
                return uIControl->GetSize();
            } else {
                return CVec2ui(0);
            }
        }

    } // namepsace ui

} // namespace drash
