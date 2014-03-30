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

    class Texture;
    class Camera;
    class Greng;
    class PointLight;
    class SpotLight;
    class Mesh;
    class ShaderProgram;
}

namespace drash {

    class Scene;
    class Figure;
    class SceneObjectGeometry;
    class SceneObjectParams;
    class SceneObject;
    class LevelObjectDesc;
    class GeometryManager;

    class DebugRenderer final {
    public:
        inline void SetCamera(greng::Camera* _camera);
        inline greng::Camera* GetCamera() const;
        inline void SetLight(greng::PointLight* _light);
        inline greng::PointLight* GetLight() const;
        inline void SetSpotLight(greng::SpotLight* _light);

        inline void SetTexCoordsScale(float _scale);

        DebugRenderer(greng::Greng& _greng, Scene& _scene,
                      GeometryManager& _geometry_manager);

        void Render() const;

        void RenderObject(const SceneObjectGeometry& _geometry,
                          const SceneObjectParams& _params);

        /// finds objects, visible at specified postion in screen space
        /// coordinates
        /// returns nearest one
        Figure* FindFigure(const greng::Camera& _camera,
                           const Vec2f& _pos) const;
        SceneObject* FindObject(const greng::Camera& _camera,
                                const Vec2f& _pos) const;

        LevelObjectDesc*
        FindObject(const greng::Camera& _camera, const Vec2f& _pos,
                   std::function<LevelObjectDesc*(unsigned int)> _object_getter,
                   unsigned int _objects_count);

    protected:
    private:
        bool InitTextures();
        bool InitShaders();

        greng::Mesh* CreateMesh(const Vec2f* _vertices,
                                unsigned int _vertices_count, float _z,
                                float _depth) const;

        greng::Greng& greng;
        Scene& scene;
        GeometryManager& geometryManager;

        greng::ShaderProgram* shaderProgram1 = {};
        greng::ShaderProgram* shaderProgram2 = {};
        greng::Camera* camera = {};
        greng::PointLight* light = {};
        greng::SpotLight* spotLight1 = {};
        greng::Texture* texture1Diffuse = {};
        greng::Texture* texture1Normal = {};
        greng::Texture* texture1Specular = {};

        float texCoordsScale = 1.0 / 40.0;
    };

    inline void DebugRenderer::SetCamera(greng::Camera* _camera) {
        camera = _camera;
    }

    inline greng::Camera* DebugRenderer::GetCamera() const { return camera; }

    inline void DebugRenderer::SetLight(greng::PointLight* _light) {
        light = _light;
    }

    inline greng::PointLight* DebugRenderer::GetLight() const { return light; }

    inline void DebugRenderer::SetSpotLight(greng::SpotLight* _light) {
        spotLight1 = _light;
    }

    inline void DebugRenderer::SetTexCoordsScale(float _scale) {
        texCoordsScale = _scale;
    }

} // namespace drash

#endif // DRASH_DEBUGRENDERER_H
