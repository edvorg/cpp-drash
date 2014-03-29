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

#ifndef GRENG_TEXTUREMANAGER_H
#define GRENG_TEXTUREMANAGER_H

#include "../misc/objectfactory.h"

namespace greng {

    class CTexture;

    class CTextureManager {
      public:
        constexpr static unsigned int mTexturesCountLimit = 1024;

        CTextureManager();
        CTextureManager(const CTextureManager &) = delete;
        CTextureManager(CTextureManager &&) = delete;
        CTextureManager &operator=(const CTextureManager &) = delete;
        CTextureManager &operator=(CTextureManager &&) = delete;
        ~CTextureManager();

        bool Init();
        void Release();

        CTexture *CreateTexture();
        CTexture *CreateTextureFromFile(const char *_path);
        CTexture *CreateTextureDummy();
        CTexture *CreateTextureWhite();
        bool DestroyTexture(CTexture *_texture);

      protected:
      private:
        drash::CObjectFactory<CTexture> mTextureFactory;
    };

} // namespace greng

#endif // GRENG_TEXTUREMANAGER_H
