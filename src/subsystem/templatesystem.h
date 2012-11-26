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

#ifndef TEMPLATESYSTEM_H
#define TEMPLATESYSTEM_H

#include "subsystem.h"
#include "../sceneobjects.h"
#include <vector>

namespace drash
{

class CSceneObjectTemplate
{
public:
    CSceneObjectGeometry mGeometry;
    std::string mName;
protected:
private:
};

class CTemplateSystem : public CSubsystem
{
public:
    typedef CSceneObjectTemplate *CSceneObjectTemplatePtr;
    typedef std::vector<CSceneObjectTemplatePtr> SceneObjectTemplatesT;

    virtual void Release() override;

    /// template is just named collection of CSceneObject parameters
    /// we can use it to create many instances of one object at any time we wan
    CSceneObjectTemplate* CreateSceneObjectTemplate(const std::string &_name);
    CSceneObject *CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params);

    const SceneObjectTemplatesT &GetTemplates() const;

protected:
private:
    SceneObjectTemplatesT mSceneObjectTemplates;
};

}// namespace drash

#endif // TEMPLATESYSTEM_H
