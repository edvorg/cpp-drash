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
    typedef std::vector<CSceneObjectTemplate> SceneObjectTemplatesT;

    /// template is just named collection of CSceneObject parameters
    /// we can use it to create many instances of one object at any time we wan
    CSceneObjectTemplate* CreateSceneObjectTemplate(const std::string &_name);
    CSceneObject *CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params);
protected:
private:
    SceneObjectTemplatesT mSceneObjectGeometries;
};

}// namespace drash

#endif // TEMPLATESYSTEM_H
