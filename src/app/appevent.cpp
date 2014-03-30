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

#include "appevent.h"

#include "../diag/logger.h"

namespace drash {

    AppEvent::AppEvent() {}

    AppEvent::AppEvent(const AppEvent& _src) : key(_src.key) {}

    AppEvent::AppEvent(EventKey _key) : key(_key) {}

    std::string AppEvent::ToString() const {
        switch (this->key) {
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

        case EventKeySpace:
            return std::string("SPC");

        case EventKeyEscape:
            return std::string("ESC");

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

        case EventKeyWheelUp:
            return std::string("WHUP");

        case EventKeyWheelDown:
            return std::string("WHDN");

        case EventDragDrop:
            return std::string("DRDP");
        case EventDragLeave:
            return std::string("DRL");
        default:
            return std::string("<unknown key>");
        }
    }

    void AppEvent::FromString(const std::string& _str) {
        if (_str == "q") {
            this->key = EventKeyQ;
        } else if (_str == "w") {
            this->key = EventKeyW;
        } else if (_str == "e") {
            this->key = EventKeyE;
        } else if (_str == "r") {
            this->key = EventKeyR;
        } else if (_str == "a") {
            this->key = EventKeyA;
        } else if (_str == "s") {
            this->key = EventKeyS;
        } else if (_str == "d") {
            this->key = EventKeyD;
        } else if (_str == "f") {
            this->key = EventKeyF;
        } else if (_str == "z") {
            this->key = EventKeyZ;
        } else if (_str == "x") {
            this->key = EventKeyX;
        } else if (_str == "c") {
            this->key = EventKeyC;
        } else if (_str == "v") {
            this->key = EventKeyV;
        } else if (_str == "SPC") {
            this->key = EventKeySpace;
        } else if (_str == "ESC") {
            this->key = EventKeyEscape;
        } else if (_str == "C") {
            this->key = EventKeyControl;
        } else if (_str == "S") {
            this->key = EventKeyShift;
        } else if (_str == "A") {
            this->key = EventKeyAlt;
        } else if (_str == "M") {
            this->key = EventKeyMeta;
        } else if (_str == "LB") {
            this->key = EventKeyLeftButton;
        } else if (_str == "RB") {
            this->key = EventKeyRightButton;
        } else if (_str == "MB") {
            this->key = EventKeyMiddleButton;
        } else if (_str == "WHUP") {
            this->key = EventKeyWheelUp;
        } else if (_str == "WHDN") {
            this->key = EventKeyWheelDown;
        } else if (_str == "DRDP") {
            this->key = EventDragDrop;
        } else if (_str == "DRL") {
            this->key = EventDragLeave;
        } else {
            this->key = EventKeyUnknown;
        }
    }

    void AppEvent::Dump() const {
        LOG_INFO("AppEvent dump: " << (unsigned int)key);
    }

} // namespace drash
