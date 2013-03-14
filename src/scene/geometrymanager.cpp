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

#include "geometrymanager.h"

#include "../scene/scene.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"

#include <fstream>

namespace drash
{

bool CGeometryManager::Init()
{
    return true;
}

void CGeometryManager::Step(double)
{
}

void CGeometryManager::Release()
{
    for (auto i = mSceneObjectTemplates.begin(), i_e = mSceneObjectTemplates.end(); i != i_e; i++)
    {
        delete i->second;
    }
    mSceneObjectTemplates.clear();
}

CSceneObjectGeometry * CGeometryManager::CreateGeometry(const std::string & _name)
{
    if (_name == "" )
    {
        return nullptr;
    }
    else
    {
        if (mSceneObjectTemplates.find(_name) != mSceneObjectTemplates.end())
        {
            LOG_ERR("Template " << _name.c_str() << " exists in TemplateSystem");
            return nullptr;
        }
        MapSceneObjectItem elem;
        elem.second = new CSceneObjectGeometry();
        elem.first = _name;
        mSceneObjectTemplates.insert(elem);
        return elem.second;
    }
}

void CGeometryManager::DestroyGeometry(CSceneObjectGeometry * _t)
{
    for (auto i = mSceneObjectTemplates.begin(), i_e = mSceneObjectTemplates.end(); i != i_e; i++)
    {
        if (i->second == _t)
        {
            delete _t;
            mSceneObjectTemplates.erase(i);
            return;
        }
    }
}

void CGeometryManager::DestroyGeometry(const std::string & _name)
{
    auto iter = mSceneObjectTemplates.find(_name);
    if (iter != mSceneObjectTemplates.end())
    {
        delete iter->second;
        mSceneObjectTemplates.erase(iter);
    }
}

CSceneObject * CGeometryManager::CreateSceneObject(const std::string & _name, const CSceneObjectParams & _params)
{
    auto iter = mSceneObjectTemplates.find(_name);
    if (iter == mSceneObjectTemplates.end())
    {
        LOG_ERR("Object" << _name.c_str() << "not found in CTemplateSystem");
        return nullptr;
    }
    return GetScene()->CreateObject(*(iter->second), _params);
}

CSceneObjectGeometry * CGeometryManager::GetGeometry(const std::string & _name)
{
    auto iter = mSceneObjectTemplates.find(_name);
    if (iter == mSceneObjectTemplates.end())
    {
        LOG_ERR("Object " << _name.c_str() << " not found in CTemplateSystem");
        return nullptr;
    }
    else
    {
        return iter->second;
    }

}

CGeometryManager::SceneObjectTemplatesT & CGeometryManager::GetGeometries()
{
    return this->mSceneObjectTemplates;
}

bool CGeometryManager::Load()
{
    Release();

    std::ifstream in("templates.txt");

    if (in.is_open() == false)
    {
        return false;
    }

    unsigned int templates_count = 0;
    unsigned int figures_count = 0;
    unsigned int vertices_count = 0;
    float z = 0;
    float depth = 0;
    CVec2f vertex;
    std::string name = "";

    in>>templates_count;

    for (unsigned int i = 0; i < templates_count; i++)
    {
        name = "";
        figures_count = 0;

        in>>name;
        in>>figures_count;

        CSceneObjectGeometry * g = CreateGeometry(name.c_str());

        g->mFigures.resize(figures_count);

        for (unsigned int j = 0; j < figures_count; j++)
        {
            vertices_count = 0;
            z = 0;
            depth = 0;

            in>>z;
            in>>depth;
            in>>vertices_count;

            g->mFigures[j].mZ = z;
            g->mFigures[j].mDepth = depth;
            g->mFigures[j].mVertices.resize(vertices_count);

            for (unsigned int k = 0; k < vertices_count; k++)
            {
                vertex.Set(0, 0);

                in>>vertex.mX;
                in>>vertex.mY;

                g->mFigures[j].mVertices[k] = vertex;
            }
        }
    }

    return true;
}

bool CGeometryManager::Store()
{
    using std::endl;

    std::ofstream out("templates.txt");

    if (out.is_open() == false)
    {
        return false;
    }

    out<<mSceneObjectTemplates.size()<<endl;

    for (auto & i : mSceneObjectTemplates)
    {
        out<<i.first<<std::endl;

        out<<i.second->mFigures.size()<<endl;

        for (auto & j : i.second->mFigures)
        {
            out<<j.mZ<<endl;
            out<<j.mDepth<<endl;
            out<<j.mVertices.size()<<endl;

            for (auto & k : j.mVertices)
            {
                out<<k.mX<<' '<<k.mY<<endl;
            }
        }
    }

    out<<endl;

    out.close();

    return true;
}

}// namespace drash
