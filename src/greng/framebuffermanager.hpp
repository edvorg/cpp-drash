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

#ifndef GRENG_FRAMEBUFFERMANAGER_H
#define GRENG_FRAMEBUFFERMANAGER_H

#include "../misc/objectfactory.h"
#include "../misc/vec2.h"
#include <vector>

namespace greng {

    class FrameBuffer;
    class Texture;

    class FrameBufferManager final {
    public:
        constexpr static unsigned int frameBuffersCountLimit = 1024;

        FrameBufferManager();
        FrameBufferManager(const FrameBufferManager&) = delete;
        FrameBufferManager(FrameBufferManager&&) = delete;
        FrameBufferManager& operator=(const FrameBufferManager&) = delete;
        FrameBufferManager& operator=(FrameBufferManager&&) = delete;
        ~FrameBufferManager();

        FrameBuffer* CreateFrameBuffer(const Texture& _color);
        FrameBuffer* CreateFrameBuffer(const std::vector<Texture*>& _colors,
                                       const Texture& _depth);
        bool DestroyFrameBuffer(FrameBuffer* _frameBuffer);

    protected:
    private:
        drash::ObjectFactory<FrameBuffer> frameBufferFactory;
    };

} // namespace greng

#endif // GRENG_FRAMEBUFFERMANAGER_H
