#ifndef MATH_H
#define MATH_H

#include <cmath>

namespace drash
{

template< typename T >
inline const T &max( const T &_a, const T &_b )
{
    return _a > _b ? _a : _b;
}

template< typename T >
inline const T &min( const T &_a, const T &_b )
{
    return _a < _b ? _a : _b;
}

template< typename T >
inline const T &clamp( const T &_val, const T &_min, const T &_max )
{
    return max( min( _val, _max ), _min );
}

class CInterval
{
public:
    CInterval();
    CInterval( const CInterval &_interval );

    inline bool IsIntersects( const CInterval &_interval );

    CInterval &Set( int _a, int _b );
    CInterval &Set( int _c );

    int a;
    int b;
};

inline bool CInterval::IsIntersects( const drash::CInterval &_interval )
{
    return ( _interval.a <= a && a <= _interval.b ) || ( _interval.a <= b && b <= _interval.b );
}

}

#endif // MATH_H
