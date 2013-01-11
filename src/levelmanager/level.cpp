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

#include "level.h"

namespace drash
{

CSceneObjectParams *CLevel::AddObject(const std::string &_template,
                                      const std::string &_name)
{
    auto template_objects = mObjects.find(_template);

    if (template_objects == mObjects.end())
    {
        template_objects = mObjects.insert(pair<string, CLevel::LEVEL_NODE>(_template, CLevel::LEVEL_NODE())).first;
    }

    auto object = template_objects->second.find(_name);

    if (object != template_objects->second.end())
    {
        LOG_ERR("CLevel::AddObject(): object "<<_name.c_str()<<" already exists");
        return nullptr;
    }

    return &template_objects->second.insert(pair<string, CSceneObjectParams>(_name, CSceneObjectParams())).first->second;
}

} // namespace drash
