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

#ifndef DRASH_UIBUTTON_H
#define DRASH_UIBUTTON_H

#include "uiwidget.h"

namespace drash {

    namespace ui {

        class UIButton : public UIWidget {
        public:
            UIButton() = default;
            UIButton(const UIButton&) = delete;
            UIButton(UIButton&&) = delete;
            UIButton& operator=(const UIButton&) = delete;
            UIButton& operator=(UIButton&&) = delete;

            virtual void Connect(UISystem* _system) override;

            void SetClickHandler(const std::function<void()>& _handler);

        protected:
        private:
            std::function<void()> clickHandler = []() {};
            bool pressed = false;
            float time = 0;
        };

    } // namepsace ui

} // namespace drash

#endif // DRASH_UIBUTTON_H
