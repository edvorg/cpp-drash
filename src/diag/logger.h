// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <sstream>

namespace drash {

    class Logger {
    public:
        explicit Logger(std::ostream& _stream);
        ~Logger(void);
        Logger& operator<<(char _c);
        Logger& operator<<(const char* _str);
        Logger& operator<<(int _v);
        Logger& operator<<(unsigned int _v);
        Logger& operator<<(long _v);
        Logger& operator<<(unsigned long _v);
        Logger& operator<<(float _v);
        Logger& operator<<(double _v);

        inline Logger& operator<<(const std::string& _str);

        static const std::string Tail();

        static auto& shared() {
            static Logger logger(std::cerr);
            return logger;
        }

    private:
        std::ofstream out;
        std::ostream& stream;
        bool opened = false;

        static std::ostringstream tailStream;
    };

    inline Logger& Logger::operator<<(const std::string& _str) {
        return (*this) << _str.c_str();
    }

#define LOG_ERR_PREFIX std::string("[E]: ")
#define LOG_WARN_PREFIX std::string("[W]: ")
#define LOG_INFO_PREFIX std::string("[I]: ")

#ifdef DRASH_DEBUG
#define LOG_ERR(mes) Logger::shared() << LOG_ERR_PREFIX << mes << '\n';
#define LOG_WARN(mes) Logger::shared() << LOG_WARN_PREFIX << mes << '\n';
#define LOG_INFO(mes) Logger::shared() << LOG_INFO_PREFIX << mes << '\n';
#else
#define LOG_ERR(mes) Logger::shared() << LOG_ERR_PREFIX << mes << '\n';
#define LOG_WARN(mes)
#define LOG_INFO(mes)
#endif

} // namespace drash

#endif // LOGGER_H_INCLUDED
