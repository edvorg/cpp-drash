// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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
#include "../misc/color4.h"
#include <GL/glew.h>
#include <SDL/SDL_image.h>

namespace greng
{

using drash::CLogger;

CTextureManager::CTextureManager()
{
    for (unsigned int i = 0; i < mTexturesCountLimit; i++)
    {
        mTextures[i] = nullptr;
    }
}

CTextureManager::~CTextureManager()
{
    for (unsigned int i = 0; i < mTexturesCount; i++)
    {
        glDeleteTextures(1, &mTextures[i]->mTextureBufferId);
        mTextures[i]->mTextureBufferId = 0;
        delete mTextures[i];
        mTextures[i] = nullptr;
    }
}

CTexture *CTextureManager::CreateTexture()
{
    if (mTexturesCount >= mTexturesCountLimit)
    {
        LOG_ERR("CTextureManager::CreateMesh(): textures count exceedes it's limit");
        return nullptr;
    }

    CTexture *res = new CTexture();

    glGenTextures(1, &res->mTextureBufferId);

    if (res->mTextureBufferId == 0)
    {
        delete res;
        return nullptr;
    }
    else
    {
        return mTextures[mTexturesCount++] = res;
    }
}

CTexture *CTextureManager::CreateTextureFromFile(const char *_path)
{
    CTexture *res = CreateTexture();

    if (res == nullptr)
    {
        return nullptr;
    }

    SDL_Surface *s = IMG_Load(_path);

    if (s == nullptr)
    {
        glDeleteTextures(1, &res->mTextureBufferId);
        res->mTextureBufferId = 0;
        delete res;
        res = nullptr;
        return nullptr;
    }

    unsigned int comps_count = (s->format->Rmask ? 1 : 0) +
                               (s->format->Gmask ? 1 : 0) +
                               (s->format->Bmask ? 1 : 0) +
                               (s->format->Amask ? 1 : 0);

    glBindTexture(GL_TEXTURE_2D, res->mTextureBufferId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 comps_count,
                 s->w,
                 s->h,
                 0,
                 comps_count == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE,
                 s->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(s);

    return res;
}

CTexture *CTextureManager::CreateTextureDummy()
{
    CTexture *res = CreateTexture();

    if (res == nullptr)
    {
        return nullptr;
    }

    drash::CColor4f data[4];
    data[0].Set(1, 0, 0, 1);
    data[1].Set(0, 1, 0, 1);
    data[2].Set(0, 0, 1, 1);
    data[3].Set(1, 1, 1, 0.5f);

    glBindTexture(GL_TEXTURE_2D, res->mTextureBufferId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return res;
}

} // namespace greng
