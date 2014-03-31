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

#ifndef GRENG_RENDERER_H
#define GRENG_RENDERER_H

#include "../misc/vec3.h"
#include "../misc/color4.h"
#include <vector>

namespace drash {

    class Matrix4f;
}

namespace greng {

    class Camera;
    class Mesh;
    class Texture;
    class ShaderProgram;
    class PointLight;
    class SpotLight;
    class FrameBuffer;
    class Viewport;

    using drash::Vec2f;
    using drash::Vec3f;
    using drash::Color4f;

    class Renderer {
    public:
        Renderer(const Viewport& newViewport) : viewport(newViewport) {}
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void RenderMesh(
            const Mesh* _mesh, unsigned int _submesh,
            const Texture* const* _textures, unsigned int _textures_count,
            const ShaderProgram* _program, const drash::Matrix4f* _model,
            const drash::Matrix4f* _view, const drash::Matrix4f* _model_view,
            const drash::Matrix4f* _proj_matrix, const PointLight* _light,
            const SpotLight* _spot_light = nullptr,
            const Vec3f* _view_pos = nullptr,
            const FrameBuffer* _frame_buffer = nullptr);

        void Clear(const FrameBuffer* _frameBuffer, const Color4f& _color);

        /// draws triangle giving screen space coordinates (-0.5,-0.5)..(0.5,
        /// 0.5) and color
        void DrawTriangle(const Camera& _camera, const Vec2f& _p1,
                          const Vec2f& _p2, const Vec2f& _p3,
                          const Color4f& _col, bool _depth_test = false) const;

        /// draws line giving world space coordinates and color
        void DrawTriangle(const Camera& _camera, const Vec3f& _p1,
                          const Vec3f& _p2, const Vec3f& _p3,
                          const Color4f& _col, bool _depth_test = true) const;

        /// draws line giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        /// start and end points
        /// and color
        void DrawLine(const Camera& _camera, const Vec2f& _p1, const Vec2f& _p2,
                      float _width, const Color4f& _col,
                      bool _depth_test = false) const;

        /// draws line giving world space coordinates start and end points
        /// and color
        void DrawLine(const Camera& _camera, const Vec3f& _p1, const Vec3f& _p2,
                      float _width, const Color4f& _col,
                      bool _depth_test = true) const;

        /// draws lines giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        /// and color
        void DrawLines(const Camera& _camera, const std::vector<Vec2f>& lines,
                       float _width, const Color4f& _col,
                       bool _depth_test = false) const;

        /// draws lines giving world space coordinates
        /// and color
        void DrawLines(const Camera& _camera, const std::vector<Vec3f>& lines,
                       float _width, const Color4f& _col,
                       bool _depth_test = true) const;

        /// draws point giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        void DrawPoint(const Camera& _camera, const Vec2f& _p, float _size,
                       const Color4f& _col, bool _depth_test = false) const;

        /// draws point giving world space coordinates
        void DrawPoint(const Camera& _camera, const Vec3f& _p, float _size,
                       const Color4f& _col, bool _depth_test = true) const;

        void DrawChar(const Camera& _camera, const Vec2f& _pos,
                      const Vec2f& _size, bool _depth_test, char _c);

        void DrawNumber(const Camera& _camera, bool fromLeft, const Vec2f& _pos,
                        const Vec2f& _size, bool _depth_test,
                        int number);

        void DrawString(const Camera& _camera, bool fromLeft, const Vec2f& _pos,
                        const Vec2f& _size, bool _depth_test,
                        const std::string& _str);

    private:
        const FrameBuffer* lastFrameBuffer;
        const Viewport& viewport;
    };

} // namespace greng

#endif // GRENG_RENDERER_H
