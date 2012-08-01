#include "cvec2.h"

namespace drash
{

float Randf( float _min, float _max, float _step )
{
    return ::rand() %
            static_cast<int>( ( _max - _min ) / _step + 1 ) *
            _step +
            _min;
}

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

CVec2::CVec2( const b2Vec2 &_copy )
{
    this->x = _copy.x;
    this->y = _copy.y;
}

CVec2 &CVec2::Rand( float _min, float _max, float _step ) //const
{
    x = drash::Randf( _min, _max, _step );
    y = drash::Randf( _min, _max, _step );
    return *this;
}

CVec2 &CVec2::RandX( float _min, float _max, float _step )
{
    x = drash::Randf( _min, _max, _step );
    return *this;
}

CVec2 &CVec2::RandY( float _min, float _max, float _step )
{
    y = drash::Randf( _min, _max, _step );
    return *this;
}

CVec2 &CVec2::operator=(const b2Vec2 &_ver)
{
    this->x = _ver.x;
    this->y = _ver.y;
}

CVec2 &CVec2::operator*(const b2Vec2 &_ver)
{
    this->x = x*_ver.x;
    this->y = y*_ver.y;
    return *this;
}

float CVec2::Dot(const b2Vec2 &_ver) const
{
    return x*_ver.x + y*_ver.y;
}

} // namespace drash
