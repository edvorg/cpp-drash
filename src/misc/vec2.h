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

#pragma once
#ifndef CVEC2_H
#define CVEC2_H

#include <Box2D/Box2D.h>
#include "../diag/clogger.h"

namespace drash
{

float Randf( float _min, float _max, float _step = 1 );

class CVec2 :  public b2Vec2
{
public:
    CVec2();
    CVec2( float32 _val );
    CVec2( float32 _x, float32 _y );
    CVec2( const b2Vec2 &_copy);
    CVec2& Rand( float _min, float _max, float _step = 1 );

    CVec2& RandX( float _min, float _max, float _step = 1 );
    CVec2& RandY( float _min, float _max, float _step = 1 );

    CVec2 & operator=( const b2Vec2 &_ver );
    CVec2 & operator*=( const b2Vec2 &_ver );
    CVec2 & operator*=( float _val );
    CVec2 & operator+=( float _val );
    CVec2 & operator+=( const b2Vec2 &_ver );
    CVec2 & operator-=( float _val );
    CVec2 & operator-=( const b2Vec2 &_ver );
    CVec2 & operator/=( float _val );
    CVec2 & operator/=( const b2Vec2 &_ver );
    bool operator==( const CVec2 &_var );
    float Dot( const b2Vec2 &_ver ) const;

private:
    static const float mAccuracy;
};

CLogger& operator<<( CLogger& _logger, const b2Vec2 &_v );

} // namespace drash

#endif // CVEC2_H
