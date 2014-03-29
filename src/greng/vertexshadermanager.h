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

#ifndef GRENG_VERTEXSHADERMANAGER_H
#define GRENG_VERTEXSHADERMANAGER_H

#include "../misc/objectfactory.h"

namespace greng {

    class CVertexShader;

    class CVertexShaderManager {
      public:
        constexpr static unsigned int shadersCountLimit = 32;

        CVertexShaderManager();
        CVertexShaderManager(const CVertexShaderManager&) = delete;
        CVertexShaderManager(CVertexShaderManager&&) = delete;
        CVertexShaderManager& operator=(const CVertexShaderManager&) = delete;
        CVertexShaderManager& operator=(CVertexShaderManager&&) = delete;
        ~CVertexShaderManager();

        CVertexShader* CreateShader();
        CVertexShader* CreateShaderDummy();
        CVertexShader* CreateShaderFromSource(const char* _source);
        CVertexShader* CreateShaderFromFile(const char* _path);
        bool DestroyShader(CVertexShader* _shader);

      protected:
      private:
        drash::CObjectFactory<CVertexShader> shaderFactory;
    };

} // namespace greng

#endif // GRENG_VERTEXSHADERMANAGER_H
