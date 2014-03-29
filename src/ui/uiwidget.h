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

#ifndef DRASH_UIWIDGET_H
#define DRASH_UIWIDGET_H

#include <functional>
#include "../misc/vec2.h"

namespace drash {

    namespace ui {

        class CUIControl;
        class CUISystem;

        class CUIWidget {
          public:
            CUIWidget() = default;
            CUIWidget(const CUIWidget &) = delete;
            CUIWidget(CUIWidget &&) = delete;
            CUIWidget &operator=(const CUIWidget &) = delete;
            CUIWidget &operator=(CUIWidget &&) = delete;
            virtual ~CUIWidget();

            virtual void Connect(CUISystem *_system);
            virtual void Disconnect();

            CUISystem *GetUISystem() const;

            void SetPressHandler(const std::function<void()> &_handler);
            void SetReleaseHandler(const std::function<void()> &_handler);
            void
            SetStepHandler(const std::function<void(double _dt)> &_handler);
            void SetDrawHandler(const std::function<void()> &_handler);

            void SetPos(const CVec2i &_pos);
            void SetSize(const CVec2ui &_size);
            const CVec2i GetPos() const;
            const CVec2ui GetSize() const;

          protected:
          private:
            CUIControl *mUIControl = nullptr;
            CUISystem *mUISystem = nullptr;
        };

    } // namepsace ui

} // namespace drash

#endif // DRASH_UIWIDGET_H
