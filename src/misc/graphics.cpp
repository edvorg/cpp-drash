#include "graphics.h"

#include <GL/gl.h>

namespace drash
{

void DrawBodySide(const b2Vec2 &_v1,
                  const b2Vec2 &_v2,
                  float _z,
                  float _depth,
                  const b2Color &_diffuse )
{
    CVec2 dp = _v1 - _v2;
    dp.Normalize();
    CVec2 localx(1, 0);

    float dot = dp.x * localx.x + dp.y * localx.y;
    dot += 2.0;
    dot /= 3.0f;

    glColor3f( _diffuse.r * dot,
               _diffuse.g * dot,
               _diffuse.b * dot );

    glVertex3f( _v1.x,
                _v1.y,
                _z + _depth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _z - _depth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _z + _depth / 2.0f );

    glVertex3f( _v2.x,
                _v2.y,
                _z + _depth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _z - _depth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _z - _depth / 2.0f );
}

void DrawBody(const b2Vec2 *_vertices,
              unsigned int _count,
              float _z,
              float _depth,
              const b2Color &_color)
{
    glEnable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f( 0.4 * _color.r,
               0.4 * _color.g,
               0.4 * _color.b );
    for ( unsigned int i = 0; i < _count; i++ )
    {
        glVertex3f( _vertices[i].x,
                    _vertices[i].y,
                    _z + _depth / 2.0f );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f( 0.4 * _color.r,
               0.4 * _color.g,
               0.4 * _color.b );
    for ( unsigned int i = 0; i < _count; i++ )
    {
        glVertex3f( _vertices[i].x,
                    _vertices[i].y,
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
                     _color );
    }
    DrawBodySide(_vertices[_count - 1],
                 _vertices[0],
                 _z,
                 _depth,
                 _color );
    glEnd();
}

void DrawCircle(float _rad,
                float _r, float _g, float _b, float _a)
{
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLE_STRIP);
    static const unsigned int segments = 16;
    static const double delta = 2.0 * M_PI / segments;
    for (unsigned int i=0; i<segments+1; i++)
    {
        glColor4f(_r, _g, _b, _a);
        glVertex2f(0, 0);
        glColor4f(_r, _g, _b, _a);
        glVertex2f(_rad * cos(i*delta), _rad * sin(i*delta));
    }
    glEnd();

    glDisable(GL_BLEND);
}

}// namespace drash
