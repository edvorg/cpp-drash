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

    CVec2& rand( int _min, int _max, float _step );

    CVec2& randX(int _min, int _max, float _step);
    CVec2& randY(int _min, int _max, float _step);

};

} // namespace drash

#endif // CVEC2_H
