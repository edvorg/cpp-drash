#ifndef CDEBUGRENDERER_H
#define CDEBUGRENDERER_H

#include <Box2D/Box2D.h>

namespace drash{

class CDebugRenderer : public b2Draw
{
public:
    CDebugRenderer(void);
    virtual ~CDebugRenderer(void);

    bool Init(void);
    void Release(void);

    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon( const b2Vec2* _vertices,
                              int32 _vertexCount,
                              const b2Color& _color );

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon( const b2Vec2* _vertices,
                                   int32 _vertexCount,
                                   const b2Color& _color );

    /// Draw a circle.
    virtual void DrawCircle( const b2Vec2& _center,
                             float32 _radius,
                             const b2Color& _color );

    /// Draw a solid circle.
    virtual void DrawSolidCircle( const b2Vec2& _center,
                                  float32 _radius,
                                  const b2Vec2& _axis,
                                  const b2Color& _color );

    /// Draw a line segment.
    virtual void DrawSegment( const b2Vec2& _p1,
                              const b2Vec2& _p2,
                              const b2Color& _color );

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform( const b2Transform& _xf );

private:
    void ProjectionMatrix();
    void ModelViewIdentity();

    unsigned int mWidth;
    unsigned int mHeight;
};

} // namespace drash

#endif // CDEBUGRENDERER_H
