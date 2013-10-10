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

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <list>
#include "../misc/vec2.h"
#include "touchlistener.h"

#include <memory>

namespace drash {


enum TouchState : unsigned int
{
    TOUCH_PRESS,
    TOUCH_PRESSING,
    TOUCH_RELEASE,
    TOUCH_REPOSE
};


class CEventSystem
{

public:
    CEventSystem() = default;

    void UpdateTouchPos(const CVec2f &_pos);

    void Update();

    void Touch();
    void TouchRelease();

    void AddTouchListener(CTouchListener * _listener);
    void RemoveTouchListener(CTouchListener * _listener);

private:
    TouchState mTouchState = TOUCH_REPOSE;

    CVec2f mTouchPos;
    std::list<CTouchListener*> mTouchListeners;

    void SendTouchEvents();
//    using CTouchListnerPtr = std::shared_ptr<CTouchListener>;
};

} // namespace drash
#endif // EVENTSYSTEM_H
