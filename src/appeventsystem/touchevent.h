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

#ifndef TOUCHEVENT_H
#define TOUCHEVENT_H

#include "../misc/vec2.h"

namespace drash
{

class CTouchEvent
{

public:
    CTouchEvent() = delete;
    explicit CTouchEvent(const CVec2f & _point);

    inline const CVec2f & GetPoint() const;

private:
    CVec2f mPoint;

};

const CVec2f &CTouchEvent::GetPoint() const
{
    return mPoint;
}


} // namespace drash
#endif // TOUCHEVENT_H
