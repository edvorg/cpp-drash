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
#include "../misc/vec2.h"

namespace greng {

    class Texture;

    class TextureManager final {
    public:
        constexpr static unsigned int texturesCountLimit = 1024;

        TextureManager();
        TextureManager(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;
        ~TextureManager();

        Texture* CreateTexture();
        Texture* CreateTextureFromFile(const char* _path);
        Texture* CreateTextureDummy();
        Texture* CreateTextureWhite();
        Texture* CreateTexture(const drash::Vec2i& size);
        bool DestroyTexture(Texture* _texture);

    protected:
    private:
        drash::ObjectFactory<Texture> textureFactory;
    };

} // namespace greng

#endif // GRENG_TEXTUREMANAGER_H
