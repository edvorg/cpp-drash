// DRASH_LICENSE_BEGIN
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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef DRASH_APPEVENT_H
#define DRASH_APPEVENT_H

#include <string>

namespace drash
{

enum EventKey : unsigned int
{
    EventKeyUnknown = 0,
    EventKeyControl,
    EventKeyShift,
    EventKeyAlt,
    EventKeyMeta,
    EventKeyQ,
    EventKeyW,
    EventKeyE,
    EventKeyR,
    EventKeyA,
    EventKeyS,
    EventKeyD,
    EventKeyF,
    EventKeyZ,
    EventKeyX,
    EventKeyC,
    EventKeyV,
    EventKeySpace,
    EventKeyEscape,
    EventKeyLeftButton,
    EventKeyRightButton,
    EventKeyMiddleButton,
    EventKeyWheelUp,
    EventKeyWheelDown,
    EventDragDrop,
    EventDragLeave
};

class CAppEvent
{
public:
    CAppEvent();
    CAppEvent(const CAppEvent &_src);
    CAppEvent(EventKey _key);

    inline EventKey GetKey() const;

    std::string ToString() const;
    void FromString(const std::string &_str);

    inline bool operator ==(const CAppEvent &_to) const;
    inline bool operator !=(const CAppEvent &_to) const;
    inline bool operator <(const CAppEvent &_to) const;

    void Dump() const;

private:
    EventKey mKey = EventKeyUnknown;
};

inline EventKey CAppEvent::GetKey() const
{
    return mKey;
}

inline bool CAppEvent::operator ==(const CAppEvent &_to) const
{
    return mKey == _to.mKey;
}

inline bool CAppEvent::operator !=(const CAppEvent &_to) const
{
    return mKey != _to.mKey;
}

inline bool CAppEvent::operator <(const CAppEvent &_to) const
{
    return (unsigned int)mKey < (unsigned int)_to.mKey;
}

} // namespace drash

#endif // DRASH_APPEVENT_H
