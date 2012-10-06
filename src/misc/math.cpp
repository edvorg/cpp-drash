#include "math.h"

namespace drash
{

CInterval::CInterval():
    a(0),
    b(0)
{
}

CInterval::CInterval( const CInterval &_interval ):
    a( _interval.a ),
    b( _interval.b )
{
}

CInterval::CInterval( float _a, float _b )
{
    Set( _a, _b );
}

CInterval &CInterval::Set( int _a, int _b )
{
	if ( _a < _b )
	{
		a = _a;
		b = _b;
	}
	else
	{
		a = _b;
		b = _a;
	}
    return *this;
}

CInterval &CInterval::Set( int _c )
{
    a = b = _c;
    return *this;
}

}
