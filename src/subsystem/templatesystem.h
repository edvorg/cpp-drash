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

    CTemplateSystem();

    CSceneObjectTemplate* CreateSceneObjectTemplate(const std::string _name);
    CSceneObject *CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params);
protected:
private:
    SceneObjectTemplatesT mSceneObjectGeometries;
};

}// namespace drash

#endif // TEMPLATESYSTEM_H
