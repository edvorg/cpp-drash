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

#include "levelmanager.h"
#include "leveldesc.h"
#include "../scene/scene.h"
#include "../scene/sceneobject.h"
#include "../diag/logger.h"
#include "../templates/templatesystem.h"
#include <map>


namespace drash
{

using drash::CLogger;

CLevelManager::CLevelManager():
    mLevelFactory(mLevelsCountLimit, "CLevel")
{
}

CLevelManager::~CLevelManager()
{
}

bool CLevelManager::Init()
{
    if (mScene == nullptr)
    {
        return false;
    }

    Release();

    return true;
}

void CLevelManager::Release()
{
    while (mLevelFactory.EnumObjects() != 0)
    {
        DestroyLevel(mLevelFactory.GetObjects()[0]);
    }
}

CLevelDesc *CLevelManager::CreateLevel()
{
    CLevelDesc *res = mLevelFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    return res;
}

bool CLevelManager::DestroyLevel(CLevelDesc *_level)
{
    if (mLevelFactory.IsObject(_level) == false)
    {
        LOG_ERR("CLevelManager::DestroyLevel(): invalid level taken");
        return false;
    }

    mLevelFactory.DestroyObject(_level);

    return true;
}

bool CLevelManager::StartLevel(CLevelDesc *_level)
{
    if (mLevelFactory.IsObject(_level) == false)
    {
        LOG_ERR("CLevelManager::StartLevel(): invalid level taken");
        return false;
    }

    mScene->DestroyObjects();

    for (unsigned int i = 0; i < _level->EnumObjects(); i++)
    {
        CLevelObjectDesc * desc = _level->GetObjects()[i];

        CSceneObjectGeometry *g = mTemplateSystem->FindTemplate(desc->mGeometryName);

        if (g != nullptr)
        {
            auto obj = mScene->CreateObject(*g, desc->mParams);
        }
        else
        {
            LOG_ERR("CLevelManager::StartLevel(): template '"<<desc->mGeometryName<<"' doesn't exists");
        }
    }

    return true;
}

} // namespace drash
