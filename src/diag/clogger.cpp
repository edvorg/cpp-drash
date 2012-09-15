 #include "clogger.h"

 #include <iostream>

using namespace std;

namespace drash
{

class GnLogStarter
{
public:
    GnLogStarter()
    {
		ofstream out;
        out.open( "drash.log", ios_base::ate );
        out.close();
	}
} starter;

CLogger::CLogger( ostream &_stream ):
    mStream(_stream)
{
    out.open( "./drash.log", ios_base::app );

    mOpened = out.is_open();
}

CLogger::~CLogger(void)
{
    if ( mOpened == true )
	{
		out<<endl;
        mStream<<endl;
		out.close();
	}
}

CLogger& CLogger::operator<<( char _c )
{
    if ( mOpened == true )
	{
        out<<_c;
        mStream<<_c;
	}
	return (*this);
}

CLogger& CLogger::operator<<( const char* _str )
{
    if ( mOpened == true )
	{
        out<<_str;
        mStream<<_str;
	}
	return (*this);
}

CLogger& CLogger::operator<<( int _v )
{
    if ( mOpened == true )
	{
        out<<_v;
        mStream<<_v;
	}
	return (*this);
}


CLogger& CLogger::operator<<( unsigned int _v )
{
    if ( mOpened == true )
	{
        out<<_v;
        mStream<<_v;
    }
    return (*this);
}

CLogger& CLogger::operator<<( long _v )
{
    if ( mOpened == true )
    {
        out<<_v;
        mStream<<_v;
    }
    return (*this);
}


CLogger& CLogger::operator<<( unsigned long _v )
{
    if ( mOpened == true )
    {
        out<<_v;
        mStream<<_v;
    }
    return (*this);
}

CLogger& CLogger::operator<<( float _v )
{
    if ( mOpened == true )
	{
        out<<_v;
        mStream<<_v;
	}
    return (*this);
}

CLogger &CLogger::operator <<(double _v)
{
    if ( mOpened == true )
    {
        out<<_v;
        mStream<<_v;
    }
    return (*this);
}

} // namespace drash
