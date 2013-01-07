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

#include <cstdlib>

namespace drash
{

namespace math
{

template<typename T>
inline const T &Max(const T &_a, const T &_b)
{
    return _a > _b ? _a : _b;
}

template<typename T>
inline const T &Min(const T &_a, const T &_b)
{
    return _a < _b ? _a : _b;
}

template<typename T>
inline const T &Clamp(const T &_val, const T &_min, const T &_max)
{
    return Max( Min( _val, _max ), _min );
}

template<typename T>
inline T Abs(const T &_val)
{
    return _val < 0 ? -_val : _val;
}

template<typename T>
T Rand(const T &_min, const T &_max, const T &_step)
{
    return ::rand() %
           static_cast<int>( ( _max - _min ) / _step + 1 ) *
           _step +
           _min;
}

inline int Sign(double _value)
{
    return _value < 0.0 ? -1 : 1;
}

} // namespace math

} // namespace drash

#endif // MATH_H
