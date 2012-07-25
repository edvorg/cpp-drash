#include "cdebugrenderer.h"

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <iostream>

using namespace drash;

CDebugRenderer::CDebugRenderer():b2Draw()
{
    mHeight = mWidth = 1;
}

bool CDebugRenderer::Init()
{
    const SDL_VideoInfo *info =  SDL_GetVideoInfo();
    if (!info){
        return false;
    }
    mWidth = info->current_w;
    mHeight = info->current_h;

    //std::cout << mWidth << " " << mHeight << std::endl;

    return true;
}

CDebugRenderer::~CDebugRenderer()
{
}

// TODO: <dismay> release color
// TODO: <dismay> not use GL_POLYGON
void CDebugRenderer::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -(float)mWidth/2 ,(float)mWidth/2,-(float)mHeight /2 ,(float)mHeight /2 ,1.0f,-1.0f);
    glBegin(GL_POLYGON);
    for(int i =0 ; i < vertexCount ; i++){
        b2Vec2 curvertex = vertices[i];// + i;
        glVertex2f(curvertex.x, curvertex.y);
    }
    glEnd();
}

void CDebugRenderer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -(float)mWidth/2 ,(float)mWidth/2,-(float)mHeight /2 ,(float)mHeight /2,1.0f,-1.0f);
    glBegin(GL_POLYGON);
//    glColor3b();
    //glColor3f(color.r,color.g,color.b);
    for(int i =0 ; i < vertexCount ; i++){
        b2Vec2 curvertex = vertices[i];// + i;
        glVertex3f(curvertex.x, curvertex.y, 0.0f);
    }
    glEnd();
}

void CDebugRenderer::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color)
{
    glMatrixMode( GL_MODELVIEW );
    glTranslatef(center.x,center.y,0);
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glColor3f(color.r,color.g,color.b);
    glOrtho( -(float)mWidth /2 ,(float)mWidth /2,-(float)mHeight /2 ,(float)mHeight /2,1.0f,-1.0f);
    glBegin(GL_LINE_LOOP);
    //float radius = 40;
    for(int i = 1 ;i < 20 ; i++){
        float angle = (float)i/M_PI * 180;
        glVertex2f(cos(angle)*radius,sin(angle)*radius);
    }
    glEnd();
}

void CDebugRenderer::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    glMatrixMode( GL_MODELVIEW );
    glTranslatef(center.x,center.y,0);
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -(float)mWidth /2 ,(float)mWidth /2,-(float)mHeight /2 ,(float)mHeight /2,1.0f,-1.0f);
    glBegin(GL_LINE_LOOP);
    //float radius = 40;
    for(int i = 1 ;i < 20 ; i++){
        float angle = (float)i/M_PI * 180;
        glVertex2f(cos(angle)*radius,sin(angle)*radius);
    }
    glEnd();
}

void CDebugRenderer::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    glMatrixMode( GL_MODELVIEW );
//    glTranslatef(center.x,center.y);
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -(float)mWidth /2 ,(float)mWidth /2,-(float)mHeight /2 ,(float)mHeight /2,1.0f,-1.0f);
    glBegin(GL_LINE);
    glVertex2f(p1.x,p1.y);
    glVertex2f(p2.x,p2.y);
    glEnd();
}

void CDebugRenderer::DrawTransform(const b2Transform &xf)
{

}
