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

#ifndef ROOT_H
#define ROOT_H

#include "../greng/grengsystemsset.h"
#include <memory>

namespace drash
{

class CScreen;

/// root system
/// TODO: add audio and prefs systems
class CRoot
{
public:
    using CScreenPtr = std::shared_ptr<CScreen>;

    CRoot();
    virtual ~CRoot();

    void Step(double _dt);
    void Render();

    greng::CGrengSystemsSet &GetGrengSystems();
    void SetScreen(const CScreenPtr &_ptr);
    CScreenPtr GetScreen();

private:
    greng::CGrengSystemsSet mGrengSystems;
    CScreenPtr mScreen = nullptr;
};

inline greng::CGrengSystemsSet &CRoot::GetGrengSystems()
{
    return mGrengSystems;
}

inline void CRoot::SetScreen(const CScreenPtr &_ptr)
{
    mScreen = _ptr;
}

inline CRoot::CScreenPtr CRoot::GetScreen()
{
    return mScreen;
}

} // namespace drash
#endif // ROOT_H
