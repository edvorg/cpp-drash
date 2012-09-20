#pragma once
#ifndef CDEBUGRENDERER_H
#define CDEBUGRENDERER_H

#include <Box2D/Box2D.h>
#include "../sceneobjects/ccamera.h"

namespace drash{

class CDebugRenderer : public b2Draw
{
public:
    CDebugRenderer(void);
    virtual ~CDebugRenderer(void);

    bool Init(void);
    void Release(void);

    virtual void DrawPolygon( const b2Vec2 *_vertices,
                              int32 _vertexCount,
                              const b2Color &_color );

    virtual void DrawSolidPolygon( const b2Vec2 *_vertices,
                                   int32 _vertexCount,
                                   const b2Color &_color );

    virtual void DrawCircle( const b2Vec2 &_center,
                             float32 _radius,
                             const b2Color &_color );

    virtual void DrawSolidCircle( const b2Vec2 &_center,
                                  float32 _radius,
                                  const b2Vec2 &_axis,
                                  const b2Color &_color );

    virtual void DrawSegment( const b2Vec2 &_p1,
                              const b2Vec2 &_p2,
                              const b2Color &_color );

    virtual void DrawTransform( const b2Transform &_xf );

    void SetCamera( const CCamera *_camera );

private:
    void ProjectionMatrix();
    void ModelViewMatrix();

    unsigned int mWidth;
    unsigned int mHeight;
    const CCamera *mCamera;
};

} // namespace drash

#endif // CDEBUGRENDERER_H
