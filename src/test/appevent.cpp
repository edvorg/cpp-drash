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

#include "appevent.h"

#include "../diag/clogger.h"

namespace drash
{

CAppEvent::CAppEvent()
{
}

CAppEvent::CAppEvent(const CAppEvent &_src):
    mKey(_src.mKey)
{
}

CAppEvent::CAppEvent(EventKey _key):
    mKey(_key)
{
}

std::string CAppEvent::ToString() const
{
    if (this->mKey == EventKeyD)
    {
        return std::string("d");
    }
    else if (this->mKey == EventKeyF)
    {
        return std::string("f");
    }
    else if (this->mKey == EventKeyControl)
    {
        return std::string("C");
    }
    else if (this->mKey == EventKeyShift)
    {
        return std::string("S");
    }
    else if (this->mKey == EventKeyLeft)
    {
        return std::string("LB");
    }
    else if (this->mKey == EventKeyRight)
    {
        return std::string("RB");
    }
    else if (this->mKey == EventKeyMiddle)
    {
        return std::string("MB");
    }

    return std::string("");
}

void CAppEvent::FromString(const std::string &_str)
{
    if (_str == "d")
    {
        this->mKey = EventKeyD;
    }
    else if (_str == "f")
    {
        this->mKey = EventKeyF;
    }
    else if (_str == "C")
    {
        this->mKey = EventKeyControl;
    }
    else if (_str == "S")
    {
        this->mKey = EventKeyShift;
    }
    else if (_str == "LB")
    {
        this->mKey = EventKeyLeft;
    }
    else if (_str == "RB")
    {
        this->mKey = EventKeyRight;
    }
    else if (_str == "MB")
    {
        this->mKey = EventKeyMiddle;
    }
}

void CAppEvent::Dump() const
{
    LOG_INFO("CAppEvent dump: "<<(unsigned int)mKey);
}

} // namespace drash
