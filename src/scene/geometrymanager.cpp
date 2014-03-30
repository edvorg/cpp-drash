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

#include "geometrymanager.h"

#include "../scene/scene.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"

#include <fstream>

namespace drash {

    GeometryManager::GeometryManager(Scene& _scene) : scene(_scene) {}

    void GeometryManager::Step(double) {}

    GeometryManager::~GeometryManager() {
        for (auto i = sceneObjectTemplates.begin(),
                  i_e = sceneObjectTemplates.end();
             i != i_e; i++) {
            delete i->second;
        }
        sceneObjectTemplates.clear();
    }

    SceneObjectGeometry*
    GeometryManager::CreateGeometry(const std::string& _name) {
        if (_name == "") {
            return nullptr;
        } else {
            if (sceneObjectTemplates.find(_name) !=
                sceneObjectTemplates.end()) {
                LOG_ERR("Template " << _name.c_str()
                                    << " exists in TemplateSystem");
                return nullptr;
            }
            MapSceneObjectItem elem;
            elem.second = new SceneObjectGeometry();
            elem.first = _name;
            sceneObjectTemplates.insert(elem);
            return elem.second;
        }
    }

    void GeometryManager::DestroyGeometry(SceneObjectGeometry* _t) {
        for (auto i = sceneObjectTemplates.begin(),
                  i_e = sceneObjectTemplates.end();
             i != i_e; i++) {
            if (i->second == _t) {
                delete _t;
                sceneObjectTemplates.erase(i);
                return;
            }
        }
    }

    void GeometryManager::DestroyGeometry(const std::string& _name) {
        auto iter = sceneObjectTemplates.find(_name);
        if (iter != sceneObjectTemplates.end()) {
            delete iter->second;
            sceneObjectTemplates.erase(iter);
        }
    }

    SceneObject*
    GeometryManager::CreateSceneObject(const std::string& _name,
                                       const SceneObjectParams& _params) {
        auto iter = sceneObjectTemplates.find(_name);
        if (iter == sceneObjectTemplates.end()) {
            LOG_ERR("Object" << _name.c_str() << "not found in TemplateSystem");
            return nullptr;
        }
        return scene.CreateObject(*(iter->second), _params);
    }

    SceneObjectGeometry*
    GeometryManager::GetGeometry(const std::string& _name) {
        auto iter = sceneObjectTemplates.find(_name);
        if (iter == sceneObjectTemplates.end()) {
            LOG_ERR("Object " << _name.c_str()
                              << " not found in TemplateSystem");
            return nullptr;
        } else {
            return iter->second;
        }
    }

    GeometryManager::SceneObjectTemplatesT& GeometryManager::GetGeometries() {
        return this->sceneObjectTemplates;
    }

    bool GeometryManager::Load() {
        for (auto i = sceneObjectTemplates.begin(),
                  i_e = sceneObjectTemplates.end();
             i != i_e; i++) {
            delete i->second;
        }
        sceneObjectTemplates.clear();

        std::ifstream in("templates.txt");

        if (in.is_open() == false) {
            return false;
        }

        unsigned int templates_count = 0;
        unsigned int figures_count = 0;
        unsigned int vertices_count = 0;
        float z = 0;
        float depth = 0;
        Vec2f vertex;
        std::string name = "";

        in >> templates_count;

        for (unsigned int i = 0; i < templates_count; i++) {
            name = "";
            figures_count = 0;

            in >> name;
            in >> figures_count;

            SceneObjectGeometry* g = CreateGeometry(name.c_str());

            g->figures.resize(figures_count);

            for (unsigned int j = 0; j < figures_count; j++) {
                vertices_count = 0;
                z = 0;
                depth = 0;

                in >> z;
                in >> depth;
                in >> vertices_count;

                g->figures[j].z = z;
                g->figures[j].depth = depth;
                g->figures[j].vertices.resize(vertices_count);

                for (unsigned int k = 0; k < vertices_count; k++) {
                    vertex.Set(0, 0);

                    in >> vertex.x;
                    in >> vertex.y;

                    g->figures[j].vertices[k] = vertex;
                }
            }
        }

        return true;
    }

    bool GeometryManager::Store() {
        using std::endl;

        std::ofstream out("templates.txt");

        if (out.is_open() == false) {
            return false;
        }

        out << sceneObjectTemplates.size() << endl;

        for (auto& i : sceneObjectTemplates) {
            out << i.first << std::endl;

            out << i.second->figures.size() << endl;

            for (auto& j : i.second->figures) {
                out << j.z << endl;
                out << j.depth << endl;
                out << j.vertices.size() << endl;

                for (auto& k : j.vertices) {
                    out << k.x << ' ' << k.y << endl;
                }
            }
        }

        out << endl;

        out.close();

        return true;
    }

} // namespace drash
