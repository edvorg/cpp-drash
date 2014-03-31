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

#include "framebuffermanager.hpp"
#include "framebuffer.hpp"
#include "texture.h"
#include "../diag/logger.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "../misc/color4.h"

namespace greng {

    using drash::Logger;

    FrameBufferManager::FrameBufferManager()
        : frameBufferFactory(frameBuffersCountLimit, "FrameBuffer") {}

    FrameBufferManager::~FrameBufferManager() {
        while (frameBufferFactory.EnumObjects() != 0) {
            DestroyFrameBuffer(frameBufferFactory.GetObjects()[0]);
        }
    }

    FrameBuffer* FrameBufferManager::CreateFrameBuffer(const Texture& _tex) {
        FrameBuffer* res = frameBufferFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        glGenFramebuffersEXT(1, &res->frameBufferBufferId);

        if (res->frameBufferBufferId == 0) {
            frameBufferFactory.DestroyObject(res);
            LOG_ERR("can not create framebuffer");
            return nullptr;
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, res->frameBufferBufferId);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D, _tex.textureBufferId, 0);
        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT };
        glDrawBuffers(1, buffers);

        if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) !=
            GL_FRAMEBUFFER_COMPLETE_EXT)
            LOG_ERR("frame buffer status incomplete");

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        return res;
    }

    FrameBuffer* FrameBufferManager::CreateFrameBuffer(const Texture& _color,
                                                       const Texture& _depth) {
        FrameBuffer* res = frameBufferFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        glGenFramebuffersEXT(1, &res->frameBufferBufferId);

        if (res->frameBufferBufferId == 0) {
            frameBufferFactory.DestroyObject(res);
            LOG_ERR("can not create framebuffer");
            return nullptr;
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, res->frameBufferBufferId);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D, _color.textureBufferId, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                  GL_TEXTURE_2D, _depth.textureBufferId, 0);
        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT };
        glDrawBuffers(1, buffers);

        if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) !=
            GL_FRAMEBUFFER_COMPLETE_EXT)
            LOG_ERR("frame buffer status incomplete");

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        return res;
    }

    bool FrameBufferManager::DestroyFrameBuffer(FrameBuffer* _frameBuffer) {
        if (frameBufferFactory.IsObject(_frameBuffer) == false) {
            return false;
        }

        glDeleteFramebuffersEXT(1, &_frameBuffer->frameBufferBufferId);
        _frameBuffer->frameBufferBufferId = 0;

        frameBufferFactory.DestroyObject(_frameBuffer);

        return true;
    }

} // namespace greng
