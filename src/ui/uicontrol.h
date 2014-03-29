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

#ifndef DRASH_UICONTROL_H
#define DRASH_UICONTROL_H

#include <functional>
#include "../misc/vec2.h"

namespace drash {

    namespace ui {

        class CUIControl final {
          public:
            friend class CUISystem;

            CUIControl() = default;
            CUIControl(const CUIControl&) = delete;
            CUIControl(CUIControl&&) = delete;
            CUIControl& operator=(const CUIControl&) = delete;
            CUIControl& operator=(CUIControl&&) = delete;

            inline void
            SetDestroyHandler(const std::function<void()>& _handler);
            inline void SetPressHandler(const std::function<void()>& _handler);
            inline void
            SetReleaseHandler(const std::function<void()>& _handler);
            inline void
            SetStepHandler(const std::function<void(double _dt)>& _handler);
            inline void SetDrawHandler(const std::function<void()>& _handler);

            inline void SetPos(const CVec2i& _pos);
            inline void SetSize(const CVec2ui& _size);
            inline const CVec2i& GetPos() const;
            inline const CVec2ui& GetSize() const;

          protected:
          private:
            std::function<void()> destroyHandler = []() {};
            std::function<void()> pressHandler = []() {};
            std::function<void()> releaseHandler = []() {};
            std::function<void(double)> stepHandler = [](double) {};
            std::function<void()> drawHandler = []() {};

            CVec2i pos;
            CVec2ui size;
        };

        inline void
        CUIControl::SetDestroyHandler(const std::function<void()>& _handler) {
            destroyHandler = _handler;
        }

        inline void
        CUIControl::SetPressHandler(const std::function<void()>& _handler) {
            pressHandler = _handler;
        }

        inline void
        CUIControl::SetReleaseHandler(const std::function<void()>& _handler) {
            releaseHandler = _handler;
        }

        inline void CUIControl::SetStepHandler(
            const std::function<void(double _dt)>& _handler) {
            stepHandler = _handler;
        }

        inline void
        CUIControl::SetDrawHandler(const std::function<void()>& _handler) {
            drawHandler = _handler;
        }

        inline void CUIControl::SetPos(const CVec2i& _pos) { pos = _pos; }

        inline void CUIControl::SetSize(const CVec2ui& _size) { size = _size; }

        inline const CVec2i& CUIControl::GetPos() const { return pos; }

        inline const CVec2ui& CUIControl::GetSize() const { return size; }

    } // namepsace ui

} // namespace drash

#endif // DRASH_UICONTROL_H
