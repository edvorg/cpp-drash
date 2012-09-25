#include "cdebugrenderer.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <math.h>
#include "../misc/math.h"

namespace drash
{

CDebugRenderer::CDebugRenderer(void):
    b2Draw(),
    mCamera(NULL)
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
    DrawSolidPolygon( _vertices,
                      _vertexCount,
                      _color );
}

void DrawSide( const b2Vec2 &_v1, const b2Vec2 &_v2, const b2Color &_col )
{
    CVec2 dir = _v2;
    dir -= _v1;
    dir.Normalize();

    dir.x += 1.0f;
    dir.x *= 0.25;
    dir.x += 0.5;

    glBegin(GL_POLYGON);
    glColor3f( _col.r * dir.x, _col.g * dir.x, _col.b * dir.x );
    glVertex3f( _v1.x, _v1.y, -1.0 );
    glVertex3f( _v1.x, _v1.y, -4.0f );
    glVertex3f( _v2.x, _v2.y, -4.0f );
    glVertex3f( _v2.x, _v2.y, -1.0f );
    glEnd();
}

void CDebugRenderer::DrawSolidPolygon( const b2Vec2 *_vertices,
                                       int32 _vertexCount,
                                       const b2Color &_color )
{
    static const b2Color side_color( 0.55, 0.75, 0.9 );

    this->ModelViewMatrix();
    this->ProjectionMatrix();

    // TODO: do not use GL_POLYGON, use GL_TRIANGLES

    glEnable(GL_DEPTH_TEST);

    glBegin(GL_POLYGON);

    glColor3f( _color.r, _color.g, _color.b );

    for ( int i =0 ; i < _vertexCount ; i++ )
    {
        glVertex3f( _vertices[i].x, _vertices[i].y, -1.0f );
    }

    glEnd();

    for ( int i =0 ; i < _vertexCount - 1 ; i++ )
    {
        DrawSide( _vertices[i+1],
                  _vertices[i],
                  side_color );
    }

    DrawSide( _vertices[0],
              _vertices[ _vertexCount - 1 ],
              side_color );
}

void CDebugRenderer::DrawCircle( const b2Vec2 &_center,
                                 float32 _radius,
                                 const b2Color &_color )
{
}

void CDebugRenderer::DrawSolidCircle( const b2Vec2 &_center,
                                      float32 _radius,
                                      const b2Vec2 &_axis,
                                      const b2Color &_color )
{
}

void CDebugRenderer::DrawSegment( const b2Vec2 &_p1,
                                  const b2Vec2 &_p2,
                                  const b2Color &_color )
{
}

void CDebugRenderer::DrawTransform( const b2Transform &_xf )
{
}

void CDebugRenderer::SetCamera( const CCamera *_camera )
{
    mCamera = _camera;
}

void CDebugRenderer::ProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60.0f, 1366.0 / 700.0, 1.0f, 1000.0f );
}

void CDebugRenderer::ModelViewMatrix()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( mCamera != NULL )
    {
        glTranslatef( -mCamera->mPos.Get().x,
                      -mCamera->mPos.Get().y,
                      0 );

        glRotatef( 5, 1, 0, 0 );

        glTranslatef( 0, -30, 0 );

        glTranslatef( 0, 0, mCamera->GetZoom() - mCamera->m_ZoomMax - 20.0 );
    }
}

} // namespace drash
