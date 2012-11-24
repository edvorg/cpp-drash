#pragma once
#ifndef CLOGGER_H_INCLUDED
#define CLOGGER_H_INCLUDED

#include <fstream>
#include <iostream>
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

private:
    std::ofstream out;
    std::ostream &mStream;
    bool mOpened = false;
};

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
