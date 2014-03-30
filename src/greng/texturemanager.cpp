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

#include "texturemanager.h"
#include "texture.h"
#include "../diag/logger.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include "../misc/color4.h"

namespace greng {

    using drash::Logger;

    TextureManager::TextureManager()
        : textureFactory(texturesCountLimit, "Texture") {}

    TextureManager::~TextureManager() {
        while (textureFactory.EnumObjects() != 0) {
            DestroyTexture(textureFactory.GetObjects()[0]);
        }
    }

    Texture* TextureManager::CreateTexture() {
        Texture* res = textureFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        glGenTextures(1, &res->textureBufferId);

        if (res->textureBufferId == 0) {
            textureFactory.DestroyObject(res);
            res = nullptr;
        }

        return res;
    }

    Texture* TextureManager::CreateTextureFromFile(const char* _path) {
        Texture* res = CreateTexture();

        if (res == nullptr) {
            return nullptr;
        }

        SDL_Surface* s = IMG_Load(_path);

        if (s == nullptr) {
            LOG_ERR("TextureManager::CreateTextureFromFile(): unable to load "
                    "texture \""
                    << _path << "\"");
            DestroyTexture(res);
            return nullptr;
        }

        unsigned int comps_count =
            (s->format->Rmask ? 1 : 0) + (s->format->Gmask ? 1 : 0) +
            (s->format->Bmask ? 1 : 0) + (s->format->Amask ? 1 : 0);

        glBindTexture(GL_TEXTURE_2D, res->textureBufferId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, comps_count, s->w, s->h, 0,
                     comps_count == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                     s->pixels);
        glBindTexture(GL_TEXTURE_2D, 0);

        SDL_FreeSurface(s);

        return res;
    }

    Texture* TextureManager::CreateTextureDummy() {
        Texture* res = CreateTexture();

        if (res == nullptr) {
            return nullptr;
        }

        drash::Color4f data[4];
        data[0].Set(1, 0, 0, 1);
        data[1].Set(0, 1, 0, 1);
        data[2].Set(0, 0, 1, 1);
        data[3].Set(1, 1, 1, 0.5f);

        glBindTexture(GL_TEXTURE_2D, res->textureBufferId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT,
                     data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return res;
    }

    Texture* TextureManager::CreateTextureWhite() {
        Texture* res = CreateTexture();

        if (res == nullptr) {
            return nullptr;
        }

        drash::Color4f data[4];
        data[0].Set(1, 1, 1, 1);
        data[1].Set(1, 1, 1, 1);
        data[2].Set(1, 1, 1, 1);
        data[3].Set(1, 1, 1, 1);

        glBindTexture(GL_TEXTURE_2D, res->textureBufferId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT,
                     data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return res;
    }

    Texture* TextureManager::CreateTexture(const drash::Vec2i& size) {
        Texture* res = CreateTexture();

        if (res == nullptr) {
            return nullptr;
        }

        glBindTexture(GL_TEXTURE_2D, res->textureBufferId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, {});
        glBindTexture(GL_TEXTURE_2D, 0);

        return res;
    }

    bool TextureManager::DestroyTexture(Texture* _texture) {
        if (textureFactory.IsObject(_texture) == false) {
            return false;
        }

        glDeleteTextures(1, &_texture->textureBufferId);
        _texture->textureBufferId = 0;

        textureFactory.DestroyObject(_texture);

        return true;
    }

} // namespace greng
