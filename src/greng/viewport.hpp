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

#ifndef GRENG_VIEWPORT_H
#define GRENG_VIEWPORT_H

#include "../misc/vec2.h"

namespace greng {

    class Viewport;

    class Viewport {
    public:
        auto& GetLeftBottom() { return leftBottom; }
        auto& GetLeftBottom() const { return leftBottom; }
        auto& GetRightTop() { return rightTop; }
        auto& GetRightTop() const { return rightTop; }
        auto GetSize() const { return rightTop - leftBottom; }
        void SetSize(const drash::Vec2i& size) { rightTop = leftBottom + size; }
        auto GetAspectRatio() const {
            auto size = GetSize();
            return size.y ? static_cast<float>(size.x) / size.y : 1.0f;
        }

        void SetScreenSize(const drash::Vec2i& _size) {
            leftBottom = { 0, 0 };
            rightTop = _size;
        }

    private:
        drash::Vec2i leftBottom;
        drash::Vec2i rightTop;
    };

} // namespace greng

#endif // GRENG_VIEWPORT_H
