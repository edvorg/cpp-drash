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
    CVec2 & operator+=( const b2Vec2 &_ver);
    bool operator==( const CVec2 &_var);
    float Dot(const b2Vec2 &_ver)const;
private:
    static const float mAccuracy;
};

CLogger& operator<<( CLogger& _logger, const b2Vec2 &_v );

} // namespace drash

#endif // CVEC2_H
