// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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
#ifndef CLOGGER_H_INCLUDED
#define CLOGGER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <sstream>

namespace drash
{

class CLogger
{
public:
    explicit CLogger(std::ostream &_stream);
    ~CLogger(void);
    CLogger& operator<<( char _c );
    CLogger& operator<<( const char* _str );
    CLogger& operator<<( int _v );
    CLogger& operator<<( unsigned int _v );
    CLogger& operator<<( long _v );
    CLogger& operator<<( unsigned long _v );
    CLogger& operator<<( float _v );
    CLogger& operator<<( double _v );

    inline CLogger& operator<<( const std::string &_str );

    static const std::string Tail();

private:
    std::ofstream out;
    std::ostream &mStream;
    bool mOpened = false;

    static std::ostringstream mTailStream;
};

inline CLogger& CLogger::operator<<(const std::string &_str)
{
    return (*this) << _str.c_str();
}

#define LOG_ERR_PREFIX "[E]: "
#define LOG_WARN_PREFIX "[W]: "
#define LOG_INFO_PREFIX "[I]: "

#ifdef DRASH_DEBUG
#define LOG_ERR(mes) CLogger(std::cerr)<<LOG_ERR_PREFIX<<mes
#define LOG_WARN(mes) CLogger(std::cerr)<<LOG_WARN_PREFIX<<mes
#define LOG_INFO(mes) CLogger(std::cout)<<LOG_INFO_PREFIX<<mes
#else
#define LOG_ERR(mes) CLogger(std::cerr)<<LOG_ERR_PREFIX<<mes
#define LOG_WARN(mes)
#define LOG_INFO(mes)
#endif

} // namespace drash

#endif // CLOGGER_H_INCLUDED
