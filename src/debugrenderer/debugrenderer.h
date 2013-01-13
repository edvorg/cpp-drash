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

#ifndef DRASH_DEBUGRENDERER_H
#define DRASH_DEBUGRENDERER_H

#include "../greng/pointlight.h"

namespace greng
{

class CRenderer;
class CMeshManager;
class CTextureManager;
class CTexture;
class CCamera;
class CVertexShaderManager;
class CFragmentShaderManager;
class CShaderProgramManager;
class CShaderProgram;

}

namespace drash
{

class CScene;
class CFigure;

class CDebugRenderer final
{
public:
    inline void SetScene(CScene *_scene);
    inline void SetRenderer(greng::CRenderer *_renderer);
    inline void SetMeshManager(greng::CMeshManager *_mesh_manager);
    inline void SetTextureManager(greng::CTextureManager *_texture_manager);
    inline void SetVertexShaderManager(greng::CVertexShaderManager *_vertex_shader_manager);
    inline void SetFragmentShaderManager(greng::CFragmentShaderManager *_fragment_shader_manager);
    inline void SetShaderProgramManager(greng::CShaderProgramManager *_shader_program_manager);

    inline void SetCamera(greng::CCamera *_camera);
    inline greng::CCamera *GetCamera() const;
    inline void SetLight(greng::CPointLight *_light);
    inline greng::CPointLight *GetLight() const;

    inline void SetTexCoordsScale(float _scale);

    bool Init();
    void Release();
    void Render() const;

    /// finds objects, visible at specified postion in screen space coordinates
    /// returns nearest one
    CFigure *FindFigure(const greng::CCamera *_camera, const CVec2f &_pos) const;

protected:
private:
    bool InitTextures();
    bool InitShaders();

    CScene* mScene = nullptr;
    greng::CRenderer *mRenderer = nullptr;
    greng::CMeshManager *mMeshManager = nullptr;
    greng::CTextureManager *mTextureManager = nullptr;
    greng::CVertexShaderManager *mVertexShaderManager = nullptr;
    greng::CFragmentShaderManager *mFragmentShaderManager = nullptr;
    greng::CShaderProgramManager *mShaderProgramManager = nullptr;

    greng::CShaderProgram *mShaderProgram = nullptr;
    greng::CCamera *mCamera = nullptr;
    greng::CPointLight *mLight = nullptr;
    greng::CTexture *mTexture1Diffuse = nullptr;
    greng::CTexture *mTexture1Normal = nullptr;

    float mTexCoordsScale = 1.0 / 40.0;
};

inline void CDebugRenderer::SetScene(CScene *_scene)
{
    mScene = _scene;
}

inline void CDebugRenderer::SetRenderer(greng::CRenderer *_renderer)
{
    mRenderer = _renderer;
}

inline void CDebugRenderer::SetMeshManager(greng::CMeshManager *_mesh_manager)
{
    mMeshManager = _mesh_manager;
}

inline void CDebugRenderer::SetTextureManager(greng::CTextureManager *_texture_manager)
{
    mTextureManager = _texture_manager;
}

inline void CDebugRenderer::SetVertexShaderManager(greng::CVertexShaderManager *_vertex_shader_manager)
{
    mVertexShaderManager = _vertex_shader_manager;
}

inline void CDebugRenderer::SetFragmentShaderManager(greng::CFragmentShaderManager *_fragment_shader_manager)
{
    mFragmentShaderManager = _fragment_shader_manager;
}

inline void CDebugRenderer::SetShaderProgramManager(greng::CShaderProgramManager *_shader_program_manager)
{
    mShaderProgramManager = _shader_program_manager;
}

inline void CDebugRenderer::SetCamera(greng::CCamera *_camera)
{
    mCamera = _camera;
}

inline greng::CCamera *CDebugRenderer::GetCamera() const
{
    return mCamera;
}

inline void CDebugRenderer::SetLight(greng::CPointLight *_light)
{
    mLight = _light;
}

inline greng::CPointLight *CDebugRenderer::GetLight() const
{
    return mLight;
}

inline void CDebugRenderer::SetTexCoordsScale(float _scale)
{
    mTexCoordsScale = _scale;
}

} // namespace drash

#endif // DRASH_DEBUGRENDERER_H
