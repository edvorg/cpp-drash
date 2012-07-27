#ifndef CVEC2_H
#define CVEC2_H

#include <Box2D/Box2D.h>

namespace drash
{

class CVec2 :  public b2Vec2
{
public:
    CVec2();
    CVec2( float32 _val );
    CVec2( float32 _x, float32 _y );
};

} // namespace drash

#endif // CVEC2_H
