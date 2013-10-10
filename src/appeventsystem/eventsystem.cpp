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

#include "eventsystem.h"
#include "touchlistener.h"
#include "../diag/logger.h"

namespace drash {

void CEventSystem::UpdateTouchPos(const CVec2f &_pos)
{
    mTouchPos = _pos;
}

void CEventSystem::Update()
{
    SendTouchEvents();
}

void CEventSystem::Touch()
{
    mTouchState = TOUCH_PRESS;
}

void CEventSystem::TouchRelease()
{
    mTouchState = TOUCH_RELEASE;
}

void CEventSystem::AddTouchListener(CTouchListener *_listener)
{
    mTouchListeners.push_back(_listener);
}

void CEventSystem::RemoveTouchListener(CTouchListener *_listener)
{
    mTouchListeners.remove(_listener);
}

void CEventSystem::SendTouchEvents()
{
    CTouchEvent evt(mTouchPos);
    for (CTouchListener * listener : mTouchListeners) {
        switch (mTouchState) {
        case TOUCH_PRESS:
            listener->TouchPress(evt);
            break;
        case TOUCH_PRESSING:
            listener->TouchPressing(evt);
            break;
        case TOUCH_RELEASE:
            listener->TouchRelease(evt);
            break;
        case TOUCH_REPOSE:
            break;
        default:
            LOG_ERR("CEventSystem::SendTouchEvents(): Unknow touch event");
            break;
        }
    }

    switch (mTouchState) {
    case TOUCH_PRESS:
        mTouchState = TOUCH_PRESSING;
        break;
    case TOUCH_PRESSING:
        break;
    case TOUCH_RELEASE:
        mTouchState = TOUCH_REPOSE;
        break;
    case TOUCH_REPOSE:
        break;
    default:
        break;
    }

}

} // namespace drash
