#include "templatesystem.h"

namespace drash
{

CTemplateSystem::CTemplateSystem()
{
}

CSceneObjectTemplate *CTemplateSystem::CreateSceneObjectTemplate(const std::string _name)
{
    if (_name == "")
    {
        return nullptr;
    }
    else
    {
        mSceneObjectGeometries.push_back(CSceneObjectTemplate());
        mSceneObjectGeometries.back().mName = _name;
        return &mSceneObjectGeometries.back();
    }
}

CSceneObject *CTemplateSystem::CreateSceneObjectFromTemplate(const std::string &_name, const CSceneObjectParams &_params)
{
    for (auto i=mSceneObjectGeometries.begin(), i_e=mSceneObjectGeometries.end(); i!=i_e; i++)
    {
        if (i->mName == _name)
        {
            return GetScene()->CreateObject<CSceneObject>(i->mGeometry, _params);
        }
    }

    return nullptr;
}

}// namespace drash
