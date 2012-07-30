#include "cvec2.h"

namespace drash
{

CVec2::CVec2():
    b2Vec2( 0.0f, 0.0f )
{
}

CVec2::CVec2( float32 _val ):
    b2Vec2( _val, _val )
{
}

CVec2::CVec2( float32 _x, float32 _y ):
    b2Vec2( _x, _y )
{
}

CVec2 &CVec2::rand( int _min, int _max, float _step ) //const
{
    this->randX(_min,_max,_step);
    this->randY(_min,_max,_step);
    return *this;
}

CVec2 &CVec2::randX(int _min, int _max, float _step)
{
    float r = static_cast<float>(::rand()%((_max - _min)));

    Set(r/_step,this->y);

    return *this;
}

CVec2 &CVec2::randY(int _min, int _max, float _step)
{
    float r = static_cast<float>(::rand()%((_max - _min)));
    Set(this->x,r/_step);

    return *this;
}


} // namespace drash
