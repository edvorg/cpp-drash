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

#ifndef TEMPLATESYSTEM_H
#define TEMPLATESYSTEM_H

#include <map>
#include <string>

namespace drash {

    class Scene;
    class SceneObjectGeometry;
    class SceneObjectParams;
    class SceneObject;

    class GeometryManager final {
    public:
        typedef std::map<std::string, SceneObjectGeometry*>
        SceneObjectTemplatesT;
        typedef std::pair<std::string, SceneObjectGeometry*> MapSceneObjectItem;

        GeometryManager(Scene& _scene);
        ~GeometryManager();

        void Step(double);

        /// template is just named SceneObjectGeometry
        /// we can use it to create many instances of one object at any time we
        /// want
        SceneObjectGeometry* CreateGeometry(const std::string& _name);
        void DestroyGeometry(SceneObjectGeometry* _t);
        void DestroyGeometry(const std::string& _name);
        SceneObject* CreateSceneObject(const std::string& _name,
                                       const SceneObjectParams& _params);

        SceneObjectGeometry* GetGeometry(const std::string& _name);
        SceneObjectTemplatesT& GetGeometries();

        bool Load();
        bool Store();

    protected:
    private:
        SceneObjectTemplatesT sceneObjectTemplates;

        Scene& scene;
    };

} // namespace drash

#endif // TEMPLATESYSTEM_H
