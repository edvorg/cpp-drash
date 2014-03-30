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

    class CMatrix4f;
}

namespace greng {

    class CCamera;
    class CMesh;
    class CTexture;
    class CShaderProgram;
    class CPointLight;
    class CSpotLight;

    using drash::CVec2f;
    using drash::CVec3f;
    using drash::CColor4f;

    class CRenderer {
    public:
        CRenderer() = default;
        CRenderer(const CRenderer&) = delete;
        CRenderer(CRenderer&&) = delete;
        CRenderer& operator=(const CRenderer&) = delete;
        CRenderer& operator=(CRenderer&&) = delete;
        ~CRenderer();

        void RenderMesh(
            const CMesh* _mesh, unsigned int _submesh,
            const CTexture* const* _textures, unsigned int _textures_count,
            const CShaderProgram* _program, const drash::CMatrix4f* _model,
            const drash::CMatrix4f* _view, const drash::CMatrix4f* _model_view,
            const drash::CMatrix4f* _proj_matrix, const CPointLight* _light,
            const CSpotLight* _spot_light = nullptr,
            const CVec3f* _view_pos = nullptr);

        /// draws triangle giving screen space coordinates (-0.5,-0.5)..(0.5,
        /// 0.5) and color
        void DrawTriangle(const CVec2f& _p1, const CVec2f& _p2,
                          const CVec2f& _p3, const CColor4f& _col,
                          bool _depth_test = false) const;

        /// draws line giving world space coordinates and color
        void DrawTriangle(const CCamera& _camera, const CVec3f& _p1,
                          const CVec3f& _p2, const CVec3f& _p3,
                          const CColor4f& _col, bool _depth_test = true) const;

        /// draws line giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        /// start and end points
        /// and color
        void DrawLine(const CVec2f& _p1, const CVec2f& _p2, float _width,
                      const CColor4f& _col, bool _depth_test = false) const;

        /// draws line giving world space coordinates start and end points
        /// and color
        void DrawLine(const CCamera& _camera, const CVec3f& _p1,
                      const CVec3f& _p2, float _width, const CColor4f& _col,
                      bool _depth_test = true) const;

        /// draws lines giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        /// and color
        void DrawLines(const std::vector<CVec2f>& lines, float _width,
                       const CColor4f& _col, bool _depth_test = false) const;

        /// draws lines giving world space coordinates
        /// and color
        void DrawLines(const CCamera& _camera, const std::vector<CVec3f>& lines,
                       float _width, const CColor4f& _col,
                       bool _depth_test = true) const;

        /// draws point giving screen space coordinates (-0.5,-0.5)..(0.5, 0.5)
        void DrawPoint(const CVec2f& _p, float _size, const CColor4f& _col,
                       bool _depth_test = false) const;

        /// draws point giving world space coordinates
        void DrawPoint(const CCamera& _camera, const CVec3f& _p, float _size,
                       const CColor4f& _col, bool _depth_test = true) const;

        void DrawChar(const CCamera& _camera, const CVec2f& _pos,
                      const CVec2f& _size, char _c);

        void DrawNumber(const CCamera& _camera, bool fromLeft,
                        const CVec2f& _pos, const CVec2f& _size,
                        unsigned int number);

        void DrawString(const CCamera& _camera, bool fromLeft,
                        const CVec2f& _pos, const CVec2f& _size,
                        const std::string& _str);
    };

} // namespace greng

#endif // GRENG_RENDERER_H
