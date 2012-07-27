#include "cdebugrenderer.h"

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <iostream>

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

// TODO: <dismay> not use GL_POLYGON

void CDebugRenderer::DrawPolygon( const b2Vec2 *vertices,
                                  int32 vertexCount,
                                  const b2Color &color )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    glBegin(GL_POLYGON);

    glColor3f( color.r, color.g, color.b );

    for ( int i =0 ; i < vertexCount ; i++ )
    {
        b2Vec2 curvertex = vertices[i];// + i;

        glVertex2f( curvertex.x, curvertex.y );
    }

    glEnd();
}

void CDebugRenderer::DrawSolidPolygon( const b2Vec2 *vertices,
                                       int32 vertexCount,
                                       const b2Color &color )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    glBegin(GL_POLYGON);

    glColor3f( color.r, color.g, color.b );

    for ( int i =0 ; i < vertexCount ; i++ )
    {
        b2Vec2 curvertex = vertices[i];// + i;

        glVertex3f( curvertex.x, curvertex.y, 0.0f );
    }

    glEnd();
}

void CDebugRenderer::DrawCircle( const b2Vec2 &center,
                                 float32 radius,
                                 const b2Color &color )
{
    this->ModelViewIdentity();
    glTranslatef( center.x, center.y, 0 );
    this->ProjectionMatrix();

    int num_segments = 50;

    glBegin(GL_LINE_LOOP);

    glColor3f( color.r, color.g, color.b );

    for( int ii = 0; ii < num_segments; ii++ )
    {
        float theta = 2.0f * 3.1415926f * float(ii) / (float)num_segments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f( x + center.x, y + center.y );
    }

    glEnd();
}



void CDebugRenderer::DrawSolidCircle( const b2Vec2 &center,
                                      float32 radius,
                                      const b2Vec2 &axis,
                                      const b2Color &color )
{
    this->ModelViewIdentity();
    glTranslatef( center.x, center.y, 0 );
    this->ProjectionMatrix();

    float x1, y1, x2, y2;
    float angle = 0;

    x1 = center.x;
    y1 = center.y;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f( color.r, color.g, color.b );
    glVertex2f( x1, y1 );

    for ( angle = 1.0f; angle<361.0f; angle+=0.1 )
    {
        x2 = x1 + sin(angle) * radius;
        y2 = y1 + cos(angle) * radius;
        glVertex2f( x2, y2 );
    }

    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f( 1, 1, 1 );
    glVertex2f( center.x, center.y );
    glVertex2f( axis.x, axis.y );
    glEnd();
}

void CDebugRenderer::DrawSegment( const b2Vec2 &p1,
                                  const b2Vec2 &p2,
                                  const b2Color &color )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    glBegin(GL_LINE_LOOP);
    glColor3f( color.r, color.g, color.b );
    glVertex2f( p1.x, p1.y );
    glVertex2f( p2.x, p2.y );
    glEnd();
}



void CDebugRenderer::DrawTransform( const b2Transform &xf )
{
    this->ModelViewIdentity();
    this->ProjectionMatrix();

    float dx , dy;
    const float d = 120;

    dx = cos( xf.q.GetAngle() ) * d;
    dy = sin( xf.q.GetAngle() ) * d;

    b2Vec2 p2( xf.p.x + dx, xf.p.y + dy );
    b2Vec2 p3( xf.p.x + d, xf.p.y );
    DrawSegment( xf.p, p2, b2Color( 0, 1, 0 ) );
    DrawSegment( xf.p, p3, b2Color( 0, 1, 0 ) );
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
