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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef DRASH_LEVELMANAGER_H
#define DRASH_LEVELMANAGER_H

#include "../misc/objectfactory.h"

namespace drash
{

class CScene;
class CLevel;

class CLevelManager
{
public:
    static const unsigned int mLevelsCountLimit = 10;

    CLevelManager();
    ~CLevelManager();

    inline void SetScene(CScene *_scene);

    CLevel *CreateLevel();
    bool DestroyLevel(CLevel *_level);

private:
    CScene *mScene = nullptr;

    CObjectFactory<CLevel> mLevelFactory;
};

inline void CLevelManager::SetScene(CScene *_scene)
{
    mScene = _scene;
}

} // namespace drash

#endif // DRASH_LEVELMANAGER_H
