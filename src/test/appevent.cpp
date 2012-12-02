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
    switch (this->mKey)
    {
    case EventKeyQ:
        return std::string("q");

    case EventKeyW:
        return std::string("w");

    case EventKeyE:
        return std::string("e");

    case EventKeyR:
        return std::string("r");

    case EventKeyA:
        return std::string("a");

    case EventKeyS:
        return std::string("s");

    case EventKeyD:
        return std::string("d");

    case EventKeyF:
        return std::string("f");

    case EventKeyZ:
        return std::string("z");

    case EventKeyX:
        return std::string("x");

    case EventKeyC:
        return std::string("c");

    case EventKeyV:
        return std::string("v");

    case EventKeyControl:
        return std::string("C");

    case EventKeyShift:
        return std::string("S");

    case EventKeyAlt:
        return std::string("A");

    case EventKeyMeta:
        return std::string("M");

    case EventKeyLeftButton:
        return std::string("LB");

    case EventKeyRightButton:
        return std::string("RB");

    case EventKeyMiddleButton:
        return std::string("MB");

    default:
        return std::string("<unknown key>");
    }
}

void CAppEvent::FromString(const std::string &_str)
{
    if (_str == "q")
    {
        this->mKey = EventKeyQ;
    }
    else if (_str == "w")
    {
        this->mKey = EventKeyW;
    }
    else if (_str == "e")
    {
        this->mKey = EventKeyE;
    }
    else if (_str == "r")
    {
        this->mKey = EventKeyR;
    }
    else if (_str == "a")
    {
        this->mKey = EventKeyA;
    }
    else if (_str == "s")
    {
        this->mKey = EventKeyS;
    }
    else if (_str == "d")
    {
        this->mKey = EventKeyD;
    }
    else if (_str == "f")
    {
        this->mKey = EventKeyF;
    }
    else if (_str == "z")
    {
        this->mKey = EventKeyZ;
    }
    else if (_str == "x")
    {
        this->mKey = EventKeyX;
    }
    else if (_str == "c")
    {
        this->mKey = EventKeyC;
    }
    else if (_str == "v")
    {
        this->mKey = EventKeyV;
    }
    else if (_str == "C")
    {
        this->mKey = EventKeyControl;
    }
    else if (_str == "S")
    {
        this->mKey = EventKeyShift;
    }
    else if (_str == "A")
    {
        this->mKey = EventKeyAlt;
    }
    else if (_str == "M")
    {
        this->mKey = EventKeyMeta;
    }
    else if (_str == "LB")
    {
        this->mKey = EventKeyLeftButton;
    }
    else if (_str == "RB")
    {
        this->mKey = EventKeyRightButton;
    }
    else if (_str == "MB")
    {
        this->mKey = EventKeyMiddleButton;
    }
    else
    {
        this->mKey = EventKeyUnknown;
    }
}

void CAppEvent::Dump() const
{
    LOG_INFO("CAppEvent dump: "<<(unsigned int)mKey);
}

} // namespace drash
