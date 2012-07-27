#ifndef CLOGGER_H_INCLUDED
#define CLOGGER_H_INCLUDED

#include <fstream>

namespace drash
{

class CLogger
{
	public:
        CLogger(void);
        ~CLogger(void);
        CLogger& operator<<(char _c);
        CLogger& operator<<(const char* _str);
        CLogger& operator<<(int _v);
        CLogger& operator<<(unsigned int _v);
        CLogger& operator<<(float _v);

	private:
		std::ofstream out;
        bool mOpened;
};

#define LOG_ERR_PREFIX "[E]: "
#define LOG_WARN_PREFIX "[W]: "
#define LOG_INFO_PREFIX "[I]: "

// TODO: info and warning messages should no be shown in release configuration
#define LOG_ERR(mes) CLogger()<<LOG_ERR_PREFIX<<mes
#define LOG_WARN(mes) CLogger()<<LOG_WARN_PREFIX<<mes
#define LOG_INFO(mes) CLogger()<<LOG_INFO_PREFIX<<mes

}//namespace drash

#endif // CLOGGER_H_INCLUDED
