// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
    CInterval( float _a, float _b );

    inline bool IsIntersects( const CInterval &_interval ) const;

    CInterval &Set( int _a, int _b );
    CInterval &Set( int _c );

    int GetMin() const { return a; }
    int GetMax() const { return b; }

private:
    int a = 0;
    int b = 0;
};

inline bool CInterval::IsIntersects( const drash::CInterval &_interval ) const
{
    return ( _interval.a <= a && a <= _interval.b ) ||
			( _interval.a <= b && b <= _interval.b ) ||
			( a <= _interval.a && _interval.b <= b );
}

}

#endif // MATH_H
