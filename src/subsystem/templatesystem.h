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
#include <map>

namespace drash
{

//class CSceneObjectTemplate
//{
//public:
//    CSceneObjectGeometry mGeometry;
////    std::string mName = "";
//protected:
//private:
//};

class CDrashBodyTemplate
{
public:
    CDrashBodyGeometry mGeometry;
    std::string mName = "";

protected:
private:
};

class CTemplateSystem : public CSubsystem
{
public:
    //typedef CSceneObjectTemplate *CSceneObjectTemplatePtr;
//    typedef std::vector<CSceneObjectTemplatePtr> SceneObjectTemplatesT;
    typedef CDrashBodyTemplate *CDrashBodyTemplatePtr;
    typedef std::vector<CDrashBodyTemplatePtr> DrashBodyTemplatesT;

    typedef std::map<std::string,CSceneObjectGeometry*> SceneObjectTemplatesT;
    typedef std::pair<std::string,CSceneObjectGeometry*> MapItem;
    virtual void Release() override;

    /// template is just named CSceneObjectGeometry
    /// we can use it to create many instances of one object at any time we want
    CSceneObjectGeometry* CreateSceneObjectTemplate(const std::string &_name);
    void DestoySceneObjectTemplate(CSceneObjectGeometry *_t);
    void RemoveSceneObjectTemplate(const std::string &_name);
    CSceneObject *CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params);

    /// template is just named CDrashBodyGeometry
    /// we can use it to create many instances of one object at any time we want
    CDrashBodyTemplate* CreateDrashBodyTemplate(const std::string &_name);
    void DestoyDrashBodyTemplate(CDrashBodyTemplate *_t);
    CDrashBody *CreateDrashBodyFromTemplate(const std::string &_name, const CDrashBodyParams &_params);

    const SceneObjectTemplatesT &GetSceneObjectTemplates() const;
    const DrashBodyTemplatesT &GetDrashBodyTemplates() const;

    CSceneObjectGeometry * FindTemplate(const std::string & _name);

    void RenameTemplate(const std::string &_oldName, const std::string &_newName);

protected:
private:
    SceneObjectTemplatesT mSceneObjectTemplates;
    DrashBodyTemplatesT mDrashBodyTemplates;
};

}// namespace drash

#endif // TEMPLATESYSTEM_H
