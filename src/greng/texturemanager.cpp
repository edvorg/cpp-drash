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
#include <GL/glew.h>

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

    return res;
}

} // namespace greng
