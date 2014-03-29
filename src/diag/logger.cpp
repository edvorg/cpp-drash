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

#include "logger.h"

#include <iostream>

using namespace std;

namespace drash {

    class GnLogStarter {
      public:
        GnLogStarter() {
            ofstream out;
            out.open("drash.log", ios_base::ate);
            out.close();
        }
    } starter;

    std::ostringstream CLogger::tailStream;

    CLogger::CLogger(ostream& _stream) : stream(_stream) {
        out.open("./drash.log", ios_base::app);

        opened = out.is_open();

        tailStream.str("");
        tailStream.clear();
    }

    CLogger::~CLogger(void) {
        if (opened == true) {
            out << endl;
            stream << endl;
            out.close();
        }
    }

    CLogger& CLogger::operator<<(char _c) {
        if (opened == true) {
            out << _c;
            stream << _c;
            tailStream << _c;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(const char* _str) {
        if (opened == true) {
            out << _str;
            stream << _str;
            tailStream << _str;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(int _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(unsigned int _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(long _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(unsigned long _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(float _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    CLogger& CLogger::operator<<(double _v) {
        if (opened == true) {
            out << _v;
            stream << _v;
            tailStream << _v;
        }
        return (*this);
    }

    const string CLogger::Tail() { return tailStream.str(); }

} // namespace drash
