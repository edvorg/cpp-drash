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

#ifndef DRASH_UISYSTEM_H
#define DRASH_UISYSTEM_H

#include "../misc/vec2.h"

namespace greng {

    class Renderer;
}

namespace drash {

    namespace ui {

        class UIControl;

        class UISystem final {
        public:
            constexpr static const unsigned int controlsCountLimit = 10;

            UISystem(greng::Renderer& _renderer);
            UISystem(const UISystem&) = delete;
            UISystem(UISystem&&) = delete;
            UISystem& operator=(const UISystem&) = delete;
            UISystem& operator=(UISystem&&) = delete;
            ~UISystem();

            UIControl* CreateControl();
            void DestroyControl(UIControl* _control);

            void SetAspectRatio(float _ratio);
            void SetWidth(unsigned int _width);

            bool ScreenSpaceToUISpace(const Vec2f& _from, int& _x, int& _y);
            bool UISpaceToScreenSpace(int _x, int _y, Vec2f& _v);

            void SetCursorPos(int _x, int _y);
            inline int GetCursorPosX() const;
            inline int GetCursorPosY() const;

            void BeginEvent();
            void EndEvent();

            void Step(double _dt);
            void DebugDraw() const;

            auto& GetRenderer() { return renderer; }

        protected:
        private:
            UIControl* controls[controlsCountLimit];
            unsigned int controlsCount = 0;

            float aspectRatio = 1;
            unsigned int width = 1024;
            unsigned int height = 768;

            int cursorX = 0;
            int cursorY = 0;

            UIControl* pressedControl = nullptr;

            greng::Renderer& renderer;
        };

        inline int UISystem::GetCursorPosX() const { return cursorX; }
        inline int UISystem::GetCursorPosY() const { return cursorY; }

    } // namepsace ui

} // namespace drash

#endif // DRASH_UISYSTEM_H
