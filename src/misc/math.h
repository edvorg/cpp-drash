#ifndef MATH_H
#define MATH_H

#include <cmath>

namespace drash
{

template< typename T >
inline T clamp( const T &_val, const T &_min, const T &_max )
{
    return std::max( std::min( _val, _max ), _min );
}

}

#endif // MATH_H
