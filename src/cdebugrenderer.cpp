#include "cdebugrenderer.h"

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>

namespace drash
{

CDebugRenderer::CDebugRenderer(void):
    b2Draw()
{
    mHeight = mWidth = 1;
}

CDebugRenderer::~CDebugRenderer()
{
}

bool CDebugRenderer::Init(void)
{
    const SDL_VideoInfo *info =  SDL_GetVideoInfo();

    if ( info == false )
    {
        return false;
    }

    mWidth = info->current_w;
    mHeight = info->current_h;

    return true;
}

void CDebugRenderer::Release(void)
{
}

void CDebugRenderer::DrawPolygon( const b2Vec2 *_vertices,
                                  int32 _vertexCount,
                                  const b2Color &_color )
{
    if ( _vertexCount > 1 )
    {
        for ( int i = 1 ; i < _vertexCount; i++ )
        {
            DrawSegment( _vertices[i-1], _vertices[i], _color );
        }

        DrawSegment( _vertices[0], _vertices[_vertexCount-1], _color );
    }
}

void CDebugRenderer::DrawSolidPolygon( const b2Vec2 *_vertices,
                                       int32 _vertexCount,
                                       const b2Color &_color )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    // TODO: do not use GL_POLYGON, use GL_TRIANGLES

    glBegin(GL_POLYGON);

    glColor3f( _color.r, _color.g, _color.b );

    for ( int i =0 ; i < _vertexCount ; i++ )
    {
        b2Vec2 curvertex = _vertices[i];// + i;

        glVertex3f( curvertex.x, curvertex.y, 0.0f );
    }

    glEnd();
}

void CDebugRenderer::DrawCircle( const b2Vec2 &_center,
                                 float32 _radius,
                                 const b2Color &_color )
{
    this->ModelViewIdentity();
    glTranslatef( _center.x, _center.y, 0 );
    this->ProjectionMatrix();

    int num_segments = 50;

    glBegin(GL_LINE_LOOP);

    glColor3f( _color.r, _color.g, _color.b );

    for( int ii = 0; ii < num_segments; ii++ )
    {
        float theta = 2.0f * 3.1415926f * float(ii) / (float)num_segments;
        float x = _radius * cos(theta);
        float y = _radius * sin(theta);
        glVertex2f( x + _center.x, y + _center.y );
    }

    glEnd();
}



void CDebugRenderer::DrawSolidCircle( const b2Vec2 &_center,
                                      float32 _radius,
                                      const b2Vec2 &_axis,
                                      const b2Color &_color )
{
    this->ModelViewIdentity();
    glTranslatef( _center.x, _center.y, 0 );
    this->ProjectionMatrix();

    float x1, y1, x2, y2;
    float angle = 0;

    x1 = _center.x;
    y1 = _center.y;

    // TODO: use only GL_TRIANGLES, not GL_TRIANGLE_FAN

    glBegin(GL_TRIANGLE_FAN);
    glColor3f( _color.r, _color.g, _color.b );
    glVertex2f( x1, y1 );

    for ( angle = 1.0f; angle<361.0f; angle+=0.1 )
    {
        x2 = x1 + sin(angle) * _radius;
        y2 = y1 + cos(angle) * _radius;
        glVertex2f( x2, y2 );
    }

    glEnd();

    b2Vec2 axis = _axis;
    axis.Normalize();
    axis *= _radius;

    DrawSegment( _center, axis, b2Color( 1.0f - _color.r,
                                         1.0f - _color.g,
                                         1.0f - _color.b ) );
}

void CDebugRenderer::DrawSegment( const b2Vec2 &_p1,
                                  const b2Vec2 &_p2,
                                  const b2Color &_color )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    glBegin(GL_LINES);
    glColor3f( _color.r, _color.g, _color.b );
    glVertex2f( _p1.x, _p1.y );
    glVertex2f( _p2.x, _p2.y );
    glEnd();
}



void CDebugRenderer::DrawTransform( const b2Transform &_xf )
{
    b2Vec2 p;
    float angle = _xf.q.GetAngle();
    const float size = mWidth / 20.0f;
    float cs = cos(angle) * size;
    float sn = sin(angle) * size;

    p.x = _xf.p.x + cs;
    p.y = _xf.p.y + sn;

    DrawSegment( _xf.p, p, b2Color( 1, 0, 0 ) );

    p.x = _xf.p.x - sn;
    p.y = _xf.p.y + cs;

    DrawSegment( _xf.p, p, b2Color( 0, 1, 0 ) );
}

void CDebugRenderer::ProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -(float)mWidth / 2,
             (float)mWidth / 2,
             -(float)mHeight /2,
             (float)mHeight /2,
             1.0f,
             -1.0f );
}

void CDebugRenderer::ModelViewIdentity()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

} // namespace drash
