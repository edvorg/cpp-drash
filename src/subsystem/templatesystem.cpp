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

#include "templatesystem.h"

namespace drash
{

void CTemplateSystem::Release()
{
    for (auto i=mSceneObjectTemplates.begin(), i_e=mSceneObjectTemplates.end(); i!=i_e; i++)
    {
        delete *i;
    }
    mSceneObjectTemplates.clear();
}

CSceneObjectTemplate *CTemplateSystem::CreateSceneObjectTemplate(const std::string &_name)
{
    if (_name == "")
    {
        return nullptr;
    }
    else
    {
        mSceneObjectTemplates.push_back(new CSceneObjectTemplate());
        mSceneObjectTemplates.back()->mName = _name;
        return mSceneObjectTemplates.back();
    }
}

void CTemplateSystem::DestoySceneObjectTemplate(CSceneObjectTemplate *_t)
{
    for (auto i=mSceneObjectTemplates.begin(), i_e=mSceneObjectTemplates.end(); i!=i_e; i++)
    {
        if ((*i) == _t)
        {
            delete _t;
            mSceneObjectTemplates.erase(i);
            return;
        }
    }
}

CSceneObject *CTemplateSystem::CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params)
{
    for (auto i=mSceneObjectTemplates.begin(), i_e=mSceneObjectTemplates.end(); i!=i_e; i++)
    {
        if ((*i)->mName == _name)
        {
            return GetScene()->CreateObject<CSceneObject>((*i)->mGeometry, _params);
        }
    }

    return nullptr;
}

const CTemplateSystem::SceneObjectTemplatesT &CTemplateSystem::GetTemplates() const
{
    return this->mSceneObjectTemplates;
}

}// namespace drash
