// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

namespace greng
{

class CGrengSystemsSet
{
public:
    CGrengSystemsSet() = default;

    bool Init();
    void Step(double _dt);
    void Release();

    inline greng::CMeshManager &GetMeshManager();
    inline greng::CTextureManager &GetTextureManager();
    inline greng::CVertexShaderManager &GetVertexShaderManager();
    inline greng::CFragmentShaderManager &GetFragmentShaderManager();
    inline greng::CShaderProgramManager &GetShaderProgramManager();
    inline greng::CRenderer &GetRenderer();
    inline greng::CCameraManager &GetCameraManager();

private:
    greng::CMeshManager mMeshManager;
    greng::CTextureManager mTextureManager;
    greng::CVertexShaderManager mVertexShaderManager;
    greng::CFragmentShaderManager mFragmentShaderManager;
    greng::CShaderProgramManager mShaderProgramManager;
    greng::CRenderer mRenderer;
    greng::CCameraManager mCameraManager;
};

inline greng::CMeshManager &CGrengSystemsSet::GetMeshManager()
{
    return mMeshManager;
}

inline greng::CTextureManager &CGrengSystemsSet::GetTextureManager()
{
    return mTextureManager;
}

inline greng::CVertexShaderManager &CGrengSystemsSet::GetVertexShaderManager()
{
    return mVertexShaderManager;
}

inline greng::CFragmentShaderManager &CGrengSystemsSet::GetFragmentShaderManager()
{
    return mFragmentShaderManager;
}

inline greng::CShaderProgramManager &CGrengSystemsSet::GetShaderProgramManager()
{
    return mShaderProgramManager;
}

inline greng::CRenderer &CGrengSystemsSet::GetRenderer()
{
    return mRenderer;
}

inline greng::CCameraManager &CGrengSystemsSet::GetCameraManager()
{
    return mCameraManager;
}

} // namespace greng

#endif // GRENG_GRENGSYSTEMSSET_H
