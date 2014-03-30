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

#ifndef DRASH_DEBUGRENDERER_H
#define DRASH_DEBUGRENDERER_H

#include <functional>
#include "../misc/vec2.h"

namespace greng {

    class CTexture;
    class CCamera;
    class CGrengSystemsSet;
    class CPointLight;
    class CSpotLight;
    class CMesh;
    class CShaderProgram;
}

namespace drash {

    class CScene;
    class CFigure;
    class CSceneObjectGeometry;
    class CSceneObjectParams;
    class CSceneObject;
    class CLevelObjectDesc;
    class CGeometryManager;

    class CDebugRenderer final {
    public:
        inline void SetCamera(greng::CCamera* _camera);
        inline greng::CCamera* GetCamera() const;
        inline void SetLight(greng::CPointLight* _light);
        inline greng::CPointLight* GetLight() const;
        inline void SetSpotLight(greng::CSpotLight* _light);

        inline void SetTexCoordsScale(float _scale);

        CDebugRenderer(greng::CGrengSystemsSet& _greng_systems, CScene& _scene,
                       CGeometryManager& _geometry_manager);

        void Render() const;

        void RenderObject(const CSceneObjectGeometry& _geometry,
                          const CSceneObjectParams& _params);

        /// finds objects, visible at specified postion in screen space
        /// coordinates
        /// returns nearest one
        CFigure* FindFigure(const greng::CCamera& _camera,
                            const CVec2f& _pos) const;
        CSceneObject* FindObject(const greng::CCamera& _camera,
                                 const CVec2f& _pos) const;

        CLevelObjectDesc* FindObject(
            const greng::CCamera& _camera, const CVec2f& _pos,
            std::function<CLevelObjectDesc*(unsigned int)> _object_getter,
            unsigned int _objects_count);

    protected:
    private:
        bool InitTextures();
        bool InitShaders();

        greng::CMesh* CreateMesh(const CVec2f* _vertices,
                                 unsigned int _vertices_count, float _z,
                                 float _depth) const;

        greng::CGrengSystemsSet& grengSystems;
        CScene& scene;
        CGeometryManager& geometryManager;

        greng::CShaderProgram* shaderProgram1 = {};
        greng::CShaderProgram* shaderProgram2 = {};
        greng::CCamera* camera = {};
        greng::CPointLight* light = {};
        greng::CSpotLight* spotLight1 = {};
        greng::CTexture* texture1Diffuse = {};
        greng::CTexture* texture1Normal = {};
        greng::CTexture* texture1Specular = {};

        float texCoordsScale = 1.0 / 40.0;
    };

    inline void CDebugRenderer::SetCamera(greng::CCamera* _camera) {
        camera = _camera;
    }

    inline greng::CCamera* CDebugRenderer::GetCamera() const { return camera; }

    inline void CDebugRenderer::SetLight(greng::CPointLight* _light) {
        light = _light;
    }

    inline greng::CPointLight* CDebugRenderer::GetLight() const {
        return light;
    }

    inline void CDebugRenderer::SetSpotLight(greng::CSpotLight* _light) {
        spotLight1 = _light;
    }

    inline void CDebugRenderer::SetTexCoordsScale(float _scale) {
        texCoordsScale = _scale;
    }

} // namespace drash

#endif // DRASH_DEBUGRENDERER_H
