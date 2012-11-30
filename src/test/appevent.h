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

#ifndef DRASH_APPEVENT_H
#define DRASH_APPEVENT_H

#include "../misc/cvec2.h"

namespace drash
{

enum EventType : unsigned int
{
    EventUnknown = 0,
    EventMouse = 1,
    EventKeyboard = 2,
};

enum EventKey : unsigned int
{
    EventKeyUnknown = 0,
    EventKeyW,
    EventKeyA,
    EventKeyS,
    EventKeyD,
    EventKeySpace,
    EventKeyEscape,
    EventKeyControl,
    EventKeyShift,
    EventKeyAlt,
};

enum EventButton : unsigned int
{
    EventButtonUnknown = 0,
    EventButtonLeft,
    EventButtonRight,
    EventButtonMiddle,
    EventButtonWheelUp,
    EventButtonWheelDown,
};

class CAppEvent
{
public:
    CAppEvent();
    CAppEvent(const CAppEvent &_src);
    CAppEvent(EventType _type, EventKey _key);
    CAppEvent(EventType _type, EventButton _button, float _x, float _y);

    inline EventType GetType() const;
    inline EventKey GetKey() const;
    inline EventButton GetButton() const;
    inline const CVec2 &GetPos() const;

private:
    EventType mType = EventUnknown;
    EventKey mKey = EventKeyUnknown;
    EventButton mButton = EventButtonUnknown;
    CVec2 mPos = CVec2(0, 0);
};

inline EventType CAppEvent::GetType() const
{
    return mType;
}

inline EventKey CAppEvent::GetKey() const
{
    return mKey;
}

inline EventButton CAppEvent::GetButton() const
{
    return mButton;
}

inline const CVec2 &CAppEvent::GetPos() const
{
    return mPos;
}

} // namespace drash

#endif // DRASH_APPEVENT_H
