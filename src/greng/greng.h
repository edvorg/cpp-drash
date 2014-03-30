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

#ifndef GRENG_GRENG_H
#define GRENG_GRENG_H

#include "meshmanager.h"
#include "texturemanager.h"
#include "vertexshadermanager.h"
#include "fragmentshadermanager.h"
#include "shaderprogrammanager.h"
#include "renderer.h"
#include "cameramanager.h"
#include "viewport.hpp"
#include "framebuffermanager.hpp"

namespace greng {

    class Greng {
    public:
        Greng();

        void Step(double _dt);

        auto& GetMeshManager() { return meshManager; }
        auto& GetTextureManager() { return textureManager; }
        auto& GetFrameBufferManager() { return frameBufferManager; }
        auto& GetVertexShaderManager() { return vertexShaderManager; }
        auto& GetFragmentShaderManager() { return fragmentShaderManager; }
        auto& GetShaderProgramManager() { return shaderProgramManager; }
        auto& GetRenderer() { return renderer; }
        auto& GetCameraManager() { return cameraManager; }
        auto& GetViewport() { return viewport; }

    private:
        greng::Viewport viewport;
        greng::Renderer renderer;
        greng::CameraManager cameraManager;
        greng::MeshManager meshManager;
        greng::TextureManager textureManager;
        greng::FrameBufferManager frameBufferManager;
        greng::VertexShaderManager vertexShaderManager;
        greng::FragmentShaderManager fragmentShaderManager;
        greng::ShaderProgramManager shaderProgramManager;
    };

} // namespace greng

#endif // GRENG_GRENG_H
