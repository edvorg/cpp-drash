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
    mType(_src.mType),
    mKey(_src.mKey),
    mButton(_src.mButton),
    mPos(_src.mPos)
{
}

CAppEvent::CAppEvent(EventType _type, EventKey _key)
{
    if (_type == EventKeyboard)
    {
        mType = _type;
        mKey = _key;
    }
}

CAppEvent::CAppEvent(EventType _type, EventButton _button, float _x, float _y)
{
    if (_type == EventMouse)
    {
        mType = _type;
        mButton = _button;
        mPos.Set(_x, _y);
    }
}

void CAppEvent::Dump() const
{
    LOG_INFO("CAppEvent dump: "<<(unsigned int)mType<<' '<<
             (unsigned int)mKey<<' '<<(unsigned int)mButton<<' '<<mPos);
}

} // namespace drash
