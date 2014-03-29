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

#ifndef GRENG_GRENGSYSTEMSSET_H
#define GRENG_GRENGSYSTEMSSET_H

#include "meshmanager.h"
#include "texturemanager.h"
#include "vertexshadermanager.h"
#include "fragmentshadermanager.h"
#include "shaderprogrammanager.h"
#include "renderer.h"
#include "cameramanager.h"

namespace greng {

    class CGrengSystemsSet {
      public:
        CGrengSystemsSet();

        void Step(double _dt);

        inline greng::CMeshManager& GetMeshManager();
        inline greng::CTextureManager& GetTextureManager();
        inline greng::CVertexShaderManager& GetVertexShaderManager();
        inline greng::CFragmentShaderManager& GetFragmentShaderManager();
        inline greng::CShaderProgramManager& GetShaderProgramManager();
        inline greng::CRenderer& GetRenderer();
        inline greng::CCameraManager& GetCameraManager();

      private:
        greng::CRenderer renderer;
        greng::CCameraManager cameraManager;
        greng::CMeshManager meshManager;
        greng::CTextureManager textureManager;
        greng::CVertexShaderManager vertexShaderManager;
        greng::CFragmentShaderManager fragmentShaderManager;
        greng::CShaderProgramManager shaderProgramManager;
    };

    inline greng::CMeshManager& CGrengSystemsSet::GetMeshManager() {
        return meshManager;
    }

    inline greng::CTextureManager& CGrengSystemsSet::GetTextureManager() {
        return textureManager;
    }

    inline greng::CVertexShaderManager&
    CGrengSystemsSet::GetVertexShaderManager() {
        return vertexShaderManager;
    }

    inline greng::CFragmentShaderManager&
    CGrengSystemsSet::GetFragmentShaderManager() {
        return fragmentShaderManager;
    }

    inline greng::CShaderProgramManager&
    CGrengSystemsSet::GetShaderProgramManager() {
        return shaderProgramManager;
    }

    inline greng::CRenderer& CGrengSystemsSet::GetRenderer() {
        return renderer;
    }

    inline greng::CCameraManager& CGrengSystemsSet::GetCameraManager() {
        return cameraManager;
    }

} // namespace greng

#endif // GRENG_GRENGSYSTEMSSET_H
