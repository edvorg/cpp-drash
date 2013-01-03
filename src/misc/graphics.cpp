// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "graphics.h"

#ifdef __MACH__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "matrix4.h"

namespace drash
{

void DrawBodySide(const CVec2f &_v1,
                  const CVec2f &_v2,
                  float _z,
                  float _depth,
                  const CColor4f &_diffuse,
                  float _angle)
{
    CVec2f dp = _v1 - _v2;
    dp.Normalize();
    CVec4f old_localx(1, 0, 0, 1);
    CVec4f localx(1, 0, 0, 1);
    CMatrix4f m;
    MatrixRotationZ(m, _angle);
    MatrixMultiply(m, old_localx, localx);

    float dot = dp.mX * localx.mX + dp.mY * localx.mY;
    dot += 2.0;
    dot /= 3.0f;

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v1.mX,
                _v1.mY,
                _z + _depth / 2.0f );

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v1.mX,
                _v1.mY,
                _z - _depth / 2.0f );

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v2.mX,
                _v2.mY,
                _z + _depth / 2.0f );

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v2.mX,
                _v2.mY,
                _z + _depth / 2.0f );

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v1.mX,
                _v1.mY,
                _z - _depth / 2.0f );

    glColor4f(_diffuse.mR * dot,
              _diffuse.mG * dot,
              _diffuse.mB * dot,
              _diffuse.mA);
    glVertex3f( _v2.mX,
                _v2.mY,
                _z - _depth / 2.0f );
}

void DrawBody(const CVec2f *_vertices,
              unsigned int _count,
              float _z,
              float _depth,
              const CColor4f &_color,
              float _angle)
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLE_FAN);
    for ( unsigned int i = 0; i < _count; i++ )
    {
        glColor4f(0.4 * _color.mR,
                  0.4 * _color.mG,
                  0.4 * _color.mB,
                  _color.mA);
        glVertex3f( _vertices[i].mX,
                    _vertices[i].mY,
                    _z + _depth / 2.0f );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    for ( unsigned int i = 0; i < _count; i++ )
    {
        glColor4f(0.4 * _color.mR,
                  0.4 * _color.mG,
                  0.4 * _color.mB,
                  _color.mA);
        glVertex3f( _vertices[i].mX,
                    _vertices[i].mY,
                    _z - _depth / 2.0f );
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    for ( unsigned int i = 0; i < _count-1; i++ )
    {
        DrawBodySide(_vertices[i],
                     _vertices[i+1],
                     _z,
                     _depth,
                     _color,
                     _angle);
    }
    DrawBodySide(_vertices[_count - 1],
                 _vertices[0],
                 _z,
                 _depth,
                 _color,
                 _angle);
    glEnd();
}

void DrawCircle(float _rad,
                const CColor4f &_color)
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLE_STRIP);
    static const unsigned int segments = 16;
    static const double delta = 2.0 * M_PI / segments;
    for (unsigned int i=0; i<segments+1; i++)
    {
        glColor4f(_color.mR, _color.mG, _color.mB, _color.mA);
        glVertex2f(0, 0);
        glColor4f(_color.mR, _color.mG, _color.mB, _color.mA);
        glVertex2f(_rad * cos(i*delta), _rad * sin(i*delta));
    }
    glEnd();

    glDisable(GL_BLEND);
}

}// namespace drash
