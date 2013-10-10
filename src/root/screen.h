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

#ifndef SCREEN_H
#define SCREEN_H

#include "../ui/uisystem.h"

#include "../appeventsystem/eventsystem.h"

/// base class for Screen

namespace drash
{

class CRoot;

class CScreen
{

public:
    CScreen() = delete;
    explicit CScreen(CRoot & _parent);
    virtual ~CScreen();

    virtual void Step(double _dt);
    virtual void Render();

    inline ui::CUISystem &GetUISystem();
    inline CEventSystem &GetEventSystem();
    inline CRoot & GetRoot();

protected:
private:
    ui::CUISystem mUISystem;
    CEventSystem mEventSystem;
    CRoot & mRoot;
};

inline ui::CUISystem &CScreen::GetUISystem()
{
    return mUISystem;
}

inline CEventSystem &CScreen::GetEventSystem()
{
    return mEventSystem;
}

inline CRoot & CScreen::GetRoot()
{
    return mRoot;
}

} // namespace drash

#endif // SCREEN_H
