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

CVec2 &CVec2::rand( int _min, int _max, float _step ) const
{
    // TODO: realise this

    return this;
}


} // namespace drash
