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

#include <GL/glew.h>
#include <stack>
#include "renderer.h"
#include "mesh.h"
#include "texture.h"
#include "shaderprogram.h"
#include "pointlight.h"
#include "spotlight.h"
#include "camera.h"

namespace greng {

    using drash::CLogger;

    CRenderer::~CRenderer() {}

    void CRenderer::RenderMesh(
        const CMesh* _mesh, unsigned int _submesh,
        const CTexture* const* _textures, unsigned int _textures_count,
        const CShaderProgram* _program, const drash::CMatrix4f* _model,
        const drash::CMatrix4f* _view, const drash::CMatrix4f* _model_view,
        const drash::CMatrix4f* _proj_matrix, const CPointLight* _light,
        const CSpotLight* _spot_light, const CVec3f* _view_pos) {
        if (_submesh >= _mesh->mMaterialOffsets.size() - 1) {
            return;
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

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->mVertexBufferId);
        glVertexPointer(3, GL_FLOAT, sizeof(CVertex), nullptr);
        glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex),
                          reinterpret_cast<GLvoid*>(sizeof(drash::CVec3f)));
        glNormalPointer(GL_FLOAT, sizeof(CVertex),
                        reinterpret_cast<GLvoid*>(sizeof(drash::CVec3f) +
                                                  sizeof(drash::CVec2f)));
        glColorPointer(4, GL_FLOAT, sizeof(CVertex),
                       reinterpret_cast<GLvoid*>(sizeof(drash::CVec3f) +
                                                 sizeof(drash::CVec2f) +
                                                 sizeof(drash::CVec3f)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->mIndexBufferId);

        glUseProgram(_program->mProgramId);

        int tangent_loc = glGetAttribLocation(_program->mProgramId, "gTangent");

        if (tangent_loc != -1) {
            glEnableVertexAttribArray(tangent_loc);
            glVertexAttribPointer(
                tangent_loc, 3, GL_FLOAT, 1, sizeof(CVertex),
                reinterpret_cast<GLvoid*>(
                    sizeof(drash::CVec3f) + sizeof(drash::CVec2f) +
                    sizeof(drash::CVec3f) + sizeof(drash::CColor4f)));
        }

        int binormal_loc =
            glGetAttribLocation(_program->mProgramId, "gBinormal");

        if (binormal_loc != -1) {
            glEnableVertexAttribArray(binormal_loc);
            glVertexAttribPointer(
                binormal_loc, 3, GL_FLOAT, 1, sizeof(CVertex),
                reinterpret_cast<GLvoid*>(
                    sizeof(drash::CVec3f) + sizeof(drash::CVec2f) +
                    sizeof(drash::CVec3f) + sizeof(drash::CColor4f) +
                    sizeof(drash::CVec3f)));
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
                glBindTexture(GL_TEXTURE_2D, _textures[i]->mTextureBufferId);

                std::ostringstream os;
                os << "gTex" << i + 1;

                int t1loc = glGetUniformLocation(_program->mProgramId,
                                                 os.str().c_str());
                if (t1loc != -1) {
                    glUniform1i(t1loc, i);
                } else {
                    LOG_ERR("CRenderer::RenderMesh(): Unable to find "
                            << os.str().c_str() << " attribute");
                }
            }
        }

        if (_model != nullptr) {
            int mloc =
                glGetUniformLocation(_program->mProgramId, "gModelMatrix");
            if (mloc != -1) {
                glUniformMatrix4fv(mloc, 1, GL_TRUE, _model->mData);
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find gModelMatrix "
                        "attribute");
            }
        }

        if (_view_pos != nullptr) {
            int vploc =
                glGetUniformLocation(_program->mProgramId, "gViewPosition");
            if (vploc != -1) {
                glUniform3fv(vploc, 1,
                             reinterpret_cast<const GLfloat*>(_view_pos));
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find gViewPosition "
                        "attribute");
            }
        }

        if (_view != nullptr) {
            int vloc =
                glGetUniformLocation(_program->mProgramId, "gViewMatrix");
            if (vloc != -1) {
                glUniformMatrix4fv(vloc, 1, GL_TRUE, _view->mData);
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find gViewMatrix "
                        "attribute");
            }
        }

        if (_model_view != nullptr) {
            int mvloc =
                glGetUniformLocation(_program->mProgramId, "gModelViewMatrix");
            if (mvloc != -1) {
                glUniformMatrix4fv(mvloc, 1, GL_TRUE, _model_view->mData);
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find "
                        "gModelViewMatrix attribute");
            }
        }

        if (_proj_matrix != nullptr) {
            int ploc =
                glGetUniformLocation(_program->mProgramId, "gProjMatrix");
            if (ploc != -1) {
                glUniformMatrix4fv(ploc, 1, GL_TRUE, _proj_matrix->mData);
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find gProjMatrix "
                        "attribute");
            }
        }

        if (_light != nullptr) {
            int l1ploc =
                glGetUniformLocation(_program->mProgramId, "gLight1Position");
            if (l1ploc != -1) {
                glUniform3fv(l1ploc, 1, reinterpret_cast<const GLfloat*>(
                                            &_light->mPosition));
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find "
                        "gLight1Position attribute");
            }
        }

        if (_spot_light != nullptr) {
            int sl1ploc =
                glGetUniformLocation(_program->mProgramId, "gLight1Position");
            if (sl1ploc != -1) {
                glUniform3fv(sl1ploc, 1, reinterpret_cast<const GLfloat*>(
                                             &_spot_light->mPosition));
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find "
                        "gLight1Position attribute");
            }

            int sl1dloc =
                glGetUniformLocation(_program->mProgramId, "gLight1Direction");
            if (sl1dloc != -1) {
                glUniform3fv(sl1dloc, 1, reinterpret_cast<const GLfloat*>(
                                             &_spot_light->mDirection));
            } else {
                LOG_ERR("CRenderer::RenderMesh(): Unable to find "
                        "gLight1Direction attribute");
            }
        }

        glDrawElements(GL_TRIANGLES, _mesh->mMaterialOffsets[_submesh + 1] -
                                         _mesh->mMaterialOffsets[_submesh],
                       GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(
                                            sizeof(unsigned int) *
                                            _mesh->mMaterialOffsets[_submesh]));

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

    void CRenderer::DrawTriangle(const CVec2f& _p1, const CVec2f& _p2,
                                 const CVec2f& _p3, const CColor4f& _col,
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
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p1.mX, _p1.mY);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p2.mX, _p2.mY);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p3.mX, _p3.mY);
        glEnd();
    }

    void CRenderer::DrawTriangle(const CCamera& _camera, const CVec3f& _p1,
                                 const CVec3f& _p2, const CVec3f& _p3,
                                 const CColor4f& _col, bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().mData);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().mData);

        glBegin(GL_TRIANGLES);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p1.mX, _p1.mY, _p1.mZ);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p2.mX, _p2.mY, _p2.mZ);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p3.mX, _p3.mY, _p3.mZ);
        glEnd();
    }

    void CRenderer::DrawLine(const CVec2f& _p1, const CVec2f& _p2, float _width,
                             const CColor4f& _col, bool _depth_test) const {
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
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p1.mX, _p1.mY);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p2.mX, _p2.mY);
        glEnd();
    }

    void CRenderer::DrawLine(const CCamera& _camera, const CVec3f& _p1,
                             const CVec3f& _p2, float _width,
                             const CColor4f& _col, bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().mData);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().mData);

        glLineWidth(_width);

        glBegin(GL_LINES);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p1.mX, _p1.mY, _p1.mZ);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p2.mX, _p2.mY, _p2.mZ);
        glEnd();
    }

    void CRenderer::DrawLines(const std::vector<CVec2f>& lines, float _width,
                              const CColor4f& _col, bool _depth_test) const {
        if (lines.size() < 2)
            return;

        for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
            DrawLine(*(i - 1), *i, _width, _col, _depth_test);
        }
    }

    void CRenderer::DrawLines(const CCamera& _camera,
                              const std::vector<CVec3f>& lines, float _width,
                              const CColor4f& _col, bool _depth_test) const {
        if (lines.size() < 2)
            return;

        for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
            DrawLine(_camera, *(i - 1), *i, _width, _col, _depth_test);
        }
    }

    void CRenderer::DrawPoint(const CVec2f& _p, float _size,
                              const CColor4f& _col, bool _depth_test) const {
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
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex2f(_p.mX, _p.mY);
        glEnd();
    }

    void CRenderer::DrawPoint(const CCamera& _camera, const CVec3f& _p,
                              float _size, const CColor4f& _col,
                              bool _depth_test) const {
        if (_depth_test == true) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_camera.GetViewMatrixTransposed().mData);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(_camera.GetProjectionMatrixTransposed().mData);

        glPointSize(_size);

        glBegin(GL_POINTS);
        glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
        glVertex3f(_p.mX, _p.mY, _p.mZ);
        glEnd();
    }

    void CRenderer::DrawDigit(const CVec2f& _pos, const CVec2f& _size,
                              unsigned int _digit) {
        int corners = 0;
        void* vertices = nullptr;
        void* indices = nullptr;

        if (_digit == 0) {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f,  -1.0f,
                                               -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4 };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 1) {
            static const int corn = 2;
            static GLfloat verts[corn * 2] = { 1.0f, 1.0f, 1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 2) {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f,  1.0f,  1.0f,
                                               1.0f,  0.0f,  -1.0f, 0.0f,
                                               -1.0f, -1.0f, 1.0f,  -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 3) {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                                               0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,
                                               1.0f,  -1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 4) {
            static const int corn = 5;
            static GLfloat verts[corn * 2] = {
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 5) {
            static const int corn = 6;
            static GLfloat verts[corn * 2] = { 1.0f,  1.0f,  -1.0f, 1.0f,
                                               -1.0f, 0.0f,  1.0f,  0.0f,
                                               1.0f,  -1.0f, -1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 6) {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { 1.0f,  1.0f,  -1.0f, 1.0f, -1.0f,
                                               0.0f,  1.0f,  0.0f,  1.0f, -1.0f,
                                               -1.0f, -1.0f, -1.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 7) {
            static const int corn = 3;
            static GLfloat verts[corn * 2] = { -1.0f, 1.0f, 1.0f,
                                               1.0f,  1.0f, -1.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 8) {
            static const int corn = 8;
            static GLfloat verts[corn * 2] = { -1.0f, 0.0f,  -1.0f, -1.0f,
                                               1.0f,  -1.0f, 1.0f,  0.0f,
                                               -1.0f, 0.0f,  -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f,  0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, 7, };
            corners = corn;
            vertices = verts;
            indices = inds;
        } else if (_digit == 9) {
            static const int corn = 7;
            static GLfloat verts[corn * 2] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
                                               0.0f,  -1.0f, 0.0f, -1.0f, 1.0f,
                                               1.0f,  1.0f,  1.0f, 0.0f, };
            static GLubyte inds[corn] = { 0, 1, 2, 3, 4, 5, 6, };
            corners = corn;
            vertices = verts;
            indices = inds;
        }

        // render
        if (corners > 1 && vertices && indices) {
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(-100, 100, -100, 100, -100, 100);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(_pos.mX, _pos.mY, 0);
            glScalef(_size.mX, _size.mY, 1);

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

    void CRenderer::DrawNumber(bool fromLeft, const CVec2f& _pos,
                               const CVec2f& _size, unsigned int number) {
        auto pos = _pos;

        if (fromLeft) {
            std::stack<unsigned int> digits;

            do {
                digits.push(number % 10);
                number /= 10;
            } while (number);

            while (digits.size()) {
                DrawDigit(pos, _size, digits.top());
                pos.mX += _size.mX * 2.5f;
                digits.pop();
            }
        } else {
            do {
                DrawDigit(pos, _size, number % 10);
                pos.mX -= _size.mX * 2.5f;
                number /= 10;
            } while (number);
        }
    }

} // namespace greng
