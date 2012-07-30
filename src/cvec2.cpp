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

CVec2 &CVec2::Rand( int _min, int _max, float _step ) //const
{
    this->RandX( _min, _max, _step );
    this->RandY( _min, _max, _step );
    return *this;
}

CVec2 &CVec2::RandX( int _min, int _max, float _step )
{
    x = ::rand() %
            static_cast<int>( static_cast<float>( _max - _min + 1 ) / _step + 1 ) *
            _step +
            _min;

    return *this;
}

CVec2 &CVec2::RandY( int _min, int _max, float _step )
{
    y = ::rand() %
            static_cast<int>( static_cast<float>( _max - _min + 1 ) / _step + 1 ) *
            _step +
            _min;

    return *this;
}


} // namespace drash
