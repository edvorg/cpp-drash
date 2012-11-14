#include "graphics.h"

#include <GL/gl.h>

namespace drash
{

const float g_LayerWidth = 0.01f;

void DrawBodySide(const b2Vec2 &_v1,
                  const b2Vec2 &_v2,
                  const CInterval &_interval,
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
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );

    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
}

void DrawBody(const b2Vec2 *_vertices,
              unsigned int _count,
              const CInterval &_interval,
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
                    _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
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
                    _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    for ( unsigned int i = 0; i < _count-1; i++ )
    {
        DrawBodySide(_vertices[i],
                     _vertices[i+1],
                     _interval,
                     _color );
    }
    DrawBodySide(_vertices[_count - 1],
                 _vertices[0],
                 _interval,
                 _color );
    glEnd();
}

}// namespace drash
