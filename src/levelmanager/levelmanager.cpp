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

#include "levelmanager.h"
#include "leveldesc.h"
#include "../scene/scene.h"
#include "../scene/sceneobject.h"
#include "../diag/logger.h"
#include "../scene/geometrymanager.h"
#include <map>

namespace drash {

    using drash::CLogger;

    CLevelManager::CLevelManager(CScene& _scene,
                                 CGeometryManager& _geometry_manager)
        : scene(_scene), templateSystem(_geometry_manager),
          levelFactory(levelsCountLimit, "CLevel") {}

    CLevelManager::~CLevelManager() {
        while (levelFactory.EnumObjects() != 0) {
            DestroyLevel(levelFactory.GetObjects()[0]);
        }
    }

    CLevelDesc* CLevelManager::CreateLevel() {
        CLevelDesc* res = levelFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        return res;
    }

    bool CLevelManager::DestroyLevel(CLevelDesc* _level) {
        if (levelFactory.IsObject(_level) == false) {
            LOG_ERR("CLevelManager::DestroyLevel(): invalid level taken");
            return false;
        }

        levelFactory.DestroyObject(_level);

        return true;
    }

    bool CLevelManager::StartLevel(CLevelDesc* _level) {
        if (levelFactory.IsObject(_level) == false) {
            LOG_ERR("CLevelManager::StartLevel(): invalid level taken");
            return false;
        }

        scene.DestroyObjects();

        for (unsigned int i = 0; i < _level->EnumObjects(); i++) {
            CLevelObjectDesc* desc = _level->GetObjects()[i];

            CSceneObjectGeometry* g =
                templateSystem.GetGeometry(desc->geometryName);

            if (g != nullptr) {
                scene.CreateObject(*g, desc->params);
            } else {
                LOG_ERR("CLevelManager::StartLevel(): geometry '"
                        << desc->geometryName << "' doesn't exists");
            }
        }

        return true;
    }

} // namespace drash
