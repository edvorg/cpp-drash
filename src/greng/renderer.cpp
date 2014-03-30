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

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <stack>
#include "renderer.h"
#include "mesh.h"
#include "texture.h"
#include "shaderprogram.h"
#include "pointlight.h"
#include "spotlight.h"
#include "camera.h"
#include "framebuffer.hpp"
#include "viewport.hpp"

namespace greng {

    using drash::Logger;

    void Renderer::RenderMesh(
        const Mesh* _mesh, unsigned int _submesh,
        const Texture* const* _textures, unsigned int _textures_count,
        const ShaderProgram* _program, const drash::Matrix4f* _model,
        const drash::Matrix4f* _view, const drash::Matrix4f* _model_view,
        const drash::Matrix4f* _proj_matrix, const PointLight* _light,
        const SpotLight* _spot_light, const Vec3f* _view_pos,
        const FrameBuffer* _frame_buffer) {
        if (_submesh >= _mesh->materialOffsets.size() - 1) {
            return;
        }

        if (_frame_buffer != lastFrameBuffer) {
            lastFrameBuffer = _frame_buffer;
            auto bufferId =
                lastFrameBuffer ? lastFrameBuffer->frameBufferBufferId : 0;
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bufferId);
            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(viewport.GetLeftBottom().x, viewport.GetLeftBottom().y,
                       viewport.GetRightTop().x, viewport.GetRightTop().y);
        }

        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
        glDisable(GL_CULL_FACE);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->vertexBufferId);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), nullptr);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(sizeof(drash::Vec3f)));
        glNormalPointer(GL_FLOAT, sizeof(Vertex),
                        reinterpret_cast<GLvoid*>(sizeof(drash::Vec3f) +
                                                  sizeof(drash::Vec2f)));
        glColorPointer(4, GL_FLOAT, sizeof(Vertex),
                       reinterpret_cast<GLvoid*>(sizeof(drash::Vec3f) +
                                                 sizeof(drash::Vec2f) +
                                                 sizeof(drash::Vec3f)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->indexBufferId);

        glUseProgram(_program->programId);

        int tangent_loc = glGetAttribLocation(_program->programId, "gTangent");

        if (tangent_loc != -1) {
            glEnableVertexAttribArray(tangent_loc);
            glVertexAttribPointer(
                tangent_loc, 3, GL_FLOAT, 1, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(
                    sizeof(drash::Vec3f) + sizeof(drash::Vec2f) +
                    sizeof(drash::Vec3f) + sizeof(drash::Color4f)));
        }

        int binormal_loc =
            glGetAttribLocation(_program->programId, "gBinormal");

        if (binormal_loc != -1) {
            glEnableVertexAttribArray(binormal_loc);
            glVertexAttribPointer(
                binormal_loc, 3, GL_FLOAT, 1, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(
                    sizeof(drash::Vec3f) + sizeof(drash::Vec2f) +
                    sizeof(drash::Vec3f) + sizeof(drash::Color4f) +
                    sizeof(drash::Vec3f)));
        }

        static const unsigned int textures_count_limit = 3;
        _textures_count = drash::math::Min<unsigned int>(_textures_count,
                                                         textures_count_limit);

        if (_textures == nullptr) {
            for (unsigned int i = 0; i < _textures_count; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        } else {
            for (unsigned int i = 0; i < _textures_count; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, _textures[i]->textureBufferId);

                std::ostringstream os;
                os << "gTex" << i + 1;

                int t1loc =
                    glGetUniformLocation(_program->programId, os.str().c_str());
                if (t1loc != -1) {
                    glUniform1i(t1loc, i);
                } else {
                    LOG_ERR("Renderer::RenderMesh(): Unable to find "
                            << os.str().c_str() << " attribute");
                }
            }
        }

        if (_model != nullptr) {
            int mloc =
                glGetUniformLocation(_program->programId, "gModelMatrix");
            if (mloc != -1) {
                glUniformMatrix4fv(mloc, 1, GL_TRUE, _model->data);
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find gModelMatrix "
                        "attribute");
            }
        }

        if (_view_pos != nullptr) {
            int vploc =
                glGetUniformLocation(_program->programId, "gViewPosition");
            if (vploc != -1) {
                glUniform3fv(vploc, 1,
                             reinterpret_cast<const GLfloat*>(_view_pos));
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find gViewPosition "
                        "attribute");
            }
        }

        if (_view != nullptr) {
            int vloc = glGetUniformLocation(_program->programId, "gViewMatrix");
            if (vloc != -1) {
                glUniformMatrix4fv(vloc, 1, GL_TRUE, _view->data);
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find gViewMatrix "
                        "attribute");
            }
        }

        if (_model_view != nullptr) {
            int mvloc =
                glGetUniformLocation(_program->programId, "gModelViewMatrix");
            if (mvloc != -1) {
                glUniformMatrix4fv(mvloc, 1, GL_TRUE, _model_view->data);
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find "
                        "gModelViewMatrix attribute");
            }
        }

        if (_proj_matrix != nullptr) {
            int ploc = glGetUniformLocation(_program->programId, "gProjMatrix");
            if (ploc != -1) {
                glUniformMatrix4fv(ploc, 1, GL_TRUE, _proj_matrix->data);
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find gProjMatrix "
                        "attribute");
            }
        }

        if (_light != nullptr) {
            int l1ploc =
                glGetUniformLocation(_program->programId, "gLight1Position");
            if (l1ploc != -1) {
                glUniform3fv(l1ploc, 1, reinterpret_cast<const GLfloat*>(
                                            &_light->position));
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find "
                        "gLight1Position attribute");
            }
        }

        if (_spot_light != nullptr) {
            int sl1ploc =
                glGetUniformLocation(_program->programId, "gLight1Position");
            if (sl1ploc != -1) {
                glUniform3fv(sl1ploc, 1, reinterpret_cast<const GLfloat*>(
                                             &_spot_light->position));
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find "
                        "gLight1Position attribute");
            }

            int sl1dloc =
                glGetUniformLocation(_program->programId, "gLight1Direction");
            if (sl1dloc != -1) {
                glUniform3fv(sl1dloc, 1, reinterpret_cast<const GLfloat*>(
                                             &_spot_light->direction));
            } else {
                LOG_ERR("Renderer::RenderMesh(): Unable to find "
                        "gLight1Direction attribute");
            }
        }

        glDrawElements(GL_TRIANGLES, _mesh->materialOffsets[_submesh + 1] -
                                         _mesh->materialOffsets[_submesh],
                       GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(
                                            sizeof(unsigned int) *
                                            _mesh->materialOffsets[_submesh]));

        glDisableVertexAttribArray(tangent_loc);
        glDisableVertexAttribArray(binormal_loc);

        glUseProgram(0);

        for (unsigned int i = 0; i < _textures_count; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glDisable(GL_TEXTURE_2D);
    }

    void Renderer::DrawTriangle(const Vec2f& _p1, const Vec2f& _p2,
                                 const Vec2f& _p3, const Color4f& _col,
                                 bool _depth_test) const {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_TRIANGLES);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p1.x, _p1.y);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p2.x, _p2.y);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p3.x, _p3.y);
        glEnd();
    }

    void Renderer::DrawTriangle(const Camera& _camera, const Vec3f& _p1,
                                 const Vec3f& _p2, const Vec3f& _p3,
                                 const Color4f& _col, bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().data);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().data);

        glBegin(GL_TRIANGLES);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p1.x, _p1.y, _p1.z);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p2.x, _p2.y, _p2.z);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p3.x, _p3.y, _p3.z);
        glEnd();
    }

    void Renderer::DrawLine(const Vec2f& _p1, const Vec2f& _p2, float _width,
                             const Color4f& _col, bool _depth_test) const {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glLineWidth(_width);

        glBegin(GL_LINES);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p1.x, _p1.y);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p2.x, _p2.y);
        glEnd();
    }

    void Renderer::DrawLine(const Camera& _camera, const Vec3f& _p1,
                             const Vec3f& _p2, float _width,
                             const Color4f& _col, bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().data);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().data);

        glLineWidth(_width);

        glBegin(GL_LINES);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p1.x, _p1.y, _p1.z);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p2.x, _p2.y, _p2.z);
        glEnd();
    }

    void Renderer::DrawLines(const std::vector<Vec2f>& lines, float _width,
                              const Color4f& _col, bool _depth_test) const {
        if (lines.size() < 2)
            return;

        for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
            DrawLine(*(i - 1), *i, _width, _col, _depth_test);
        }
    }

    void Renderer::DrawLines(const Camera& _camera,
                              const std::vector<Vec3f>& lines, float _width,
                              const Color4f& _col, bool _depth_test) const {
        if (lines.size() < 2)
            return;

        for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
            DrawLine(_camera, *(i - 1), *i, _width, _col, _depth_test);
        }
    }

    void Renderer::DrawPoint(const Vec2f& _p, float _size,
                              const Color4f& _col, bool _depth_test) const {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(_size);

        glBegin(GL_POINTS);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex2f(_p.x, _p.y);
        glEnd();
    }

    void Renderer::DrawPoint(const Camera& _camera, const Vec3f& _p,
                              float _size, const Color4f& _col,
                              bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().data);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().data);

        glPointSize(_size);

        glBegin(GL_POINTS);
        glColor4f(_col.r, _col.g, _col.b, _col.a);
        glVertex3f(_p.x, _p.y, _p.z);
        glEnd();
    }

    void Renderer::DrawChar(const Camera& _camera, const Vec2f& _pos,
                             const Vec2f& _size, char _c) {
        int corners = 0;
        void* vertices = nullptr;
        void* indices = nullptr;

        if (_c == '0') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f,  -1.0f,
                                               -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4 };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '1') {
            static const int corn = 2;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '2') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f,  1.0f,  1.0f,
                                               1.0f,  0.0f,  -1.0f, 0.0f,
                                               -1.0f, -1.0f, 1.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '3') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                                               0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,
                                               1.0f,  -1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '4') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '5') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { 1.0f,  1.0f,  -1.0f, 1.0f,
                                               -1.0f, 0.0f,  1.0f,  0.0f,
                                               1.0f,  -1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '6') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { 1.0f,  1.0f,  -1.0f, 1.0f, -1.0f,
                                               0.0f,  1.0f,  0.0f,  1.0f, -1.0f,
                                               -1.0f, -1.0f, -1.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '7') {
            static const int corn = 3;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f, 1.0f,
                                               1.0f,  1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '8') {
            static const int corn = 8;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  -1.0f, -1.0f,
                                               1.0f,  -1.0f, 1.0f,  0.0f,
                                               -1.0f, 0.0f,  -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f,  0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, 7, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '9') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
                                               0.0f,  -1.0f, 0.0f, -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'a') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  1.0f,  0.0f,
                                               1.0f,  -1.0f, -1.0f, -1.0f,
                                               -1.0f, -0.5f, 1.0f,  -0.5f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'b') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'c') {
            static const int corn = 4;
            static GLfloat verts[corn * 2] = { 1.0f,  -1.0f, -1.0f, -1.0f,
                                               -1.0f, 0.0f,  1.0f,  0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'd') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'e') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { 1.0f,  0.0f,  -1.0f, 0.0f,
                                               -1.0f, -0.5f, 1.0f,  -0.5f,
                                               -1.0f, -0.5f, -1.0f, -1.0f,
                                               1.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'f') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                0.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'g') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { 0.0f, 1.0f,  1.0f, 1.0f,  1.0f,
                                               0.0f, -1.0f, 0.0f, -1.0f, -1.0f,
                                               1.0f, -1.0f, 1.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'h') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, -1.0f, -1.0f, 1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'i') {
            static const int corn = 2;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'j') {
            static const int corn = 3;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, 0.0f,
                                               -1.0f, 0.0f,  1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'k') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f,   -1.0f, -1.0f,
                                               -1.0f, -0.25f, 0.0f,  .0f,
                                               -1.0f, -0.25f, 0.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'l') {
            static const int corn = 2;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'm') {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, -1.0f, 0.0f,  1.0f,
                                               0.0f,  1.0f,  -1.0f, 1.0f,  0.0f,
                                               0.0f,  0.0f,  0.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'n') {
            static const int corn = 4;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, -1.0f, 0.0f,
                                               0.0f,  0.0f,  0.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'o') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'p') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = { -1.0f, -2.0f, -1.0f, 0.0f,
                                               1.0f,  0.0f,  1.0f,  -1.0f,
                                               -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'q') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                1.0f, -2.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, -1.0f, 1.0f, -1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'r') {
            static const int corn = 3;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, -1.0f,
                                               0.0f,  0.0f,  0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 's') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { 1.0f,  0.0f,  -1.0f, 0.0f,
                                               -1.0f, -0.5f, 1.0f,  -0.5f,
                                               1.0f,  -1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 't') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                0.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'u') {
            static const int corn = 4;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  -1.0f, -1.0f,
                                               1.0f,  -1.0f, 1.0f,  0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'v') {
            static const int corn = 3;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f, -0.5f,
                                               -1.0f, 0.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'w') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                -1.0f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.5f, -1.0f, 1.0f, 0.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'x') {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                -1.0f, -1.0f, 0.0f, 0.0f, -0.5f,
                -0.5f, -1.0f, 0.0f, 0.0f, -1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'y') {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  -1.0f, -1.0f,
                                               1.0f,  -1.0f, 1.0f,  0.0f,
                                               1.0f,  -2.0f, 0.0f,  -2.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == 'z') {
            static const int corn = 4;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  1.0f, 0.0f,
                                               -1.0f, -1.0f, 1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_c == '_') {
            static const int corn = 2;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, 1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, };
            corners = corn;
            vertices = verts;
            indices = inds;
        }

        // render
        if (corners > 1 && vertices && indices) {
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(-100, 100, -100 / _camera.GetAspectRatio(),
                    100 / _camera.GetAspectRatio(), -100, 100);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(_pos.x, _pos.y, 0);
            glScalef(_size.x, _size.y, 1);

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            glDrawElements(GL_LINE_STRIP, corners, GL_UNSIGNED_BYTE, indices);
            glDisableClientState(GL_VERTEX_ARRAY);

            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
        }
    }

    static float CharWidth(char _c) {
        if (_c == 'l' || _c == '1' || _c == 'i')
            return 0.5f;

        if (_c == 'f' || _c == 't' || _c == 'k' || _c == 'j' || _c == 'n' ||
            _c == 'r' || _c == 'v' || _c == 'x')
            return 1.5f;

        return 2.5f;
    }

    void Renderer::DrawNumber(const Camera& _camera, bool fromLeft,
                               const Vec2f& _pos, const Vec2f& _size,
                               unsigned int number) {
        auto pos = _pos;

        if (fromLeft) {
            std::stack<unsigned int> digits;

            do {
                digits.push(number % 10);
                number /= 10;
            } while (number);

            while (digits.size()) {
                DrawChar(_camera, pos, _size, digits.top() + '0');
                pos.x += _size.x * CharWidth(digits.top() + '0');
                digits.pop();
            }
        } else {
            do {
                DrawChar(_camera, pos, _size, number % 10 + '0');
                pos.x -= _size.x * 2.5f;
                number /= 10;
            } while (number);
        }
    }

    void Renderer::DrawString(const Camera& _camera, bool fromLeft,
                               const Vec2f& _pos, const Vec2f& _size,
                               const std::string& _str) {
        auto pos = _pos;

        if (fromLeft) {
            for (auto& c : _str) {
                DrawChar(_camera, pos, _size, c);
                pos.x += _size.x * CharWidth(c);
            }
        } else {
            for (auto i = _str.size() - 1; i > 0; --i) {
                DrawChar(_camera, pos, _size, _str[i]);
                pos.x -= _size.x * 2.5f;
            }
        }
    }

} // namespace greng
