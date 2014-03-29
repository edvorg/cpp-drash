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

namespace drash
{

class CScene;
class CSceneObjectGeometry;
class CSceneObjectParams;
class CSceneObject;

class CGeometryManager final
{
public:
    typedef std::map<std::string,CSceneObjectGeometry *> SceneObjectTemplatesT;
    typedef std::pair<std::string,CSceneObjectGeometry *> MapSceneObjectItem;

    bool Init();
    void Step(double);
    void Release();

    /// template is just named CSceneObjectGeometry
    /// we can use it to create many instances of one object at any time we want
    CSceneObjectGeometry * CreateGeometry(const std::string & _name);
    void DestroyGeometry(CSceneObjectGeometry * _t);
    void DestroyGeometry(const std::string & _name);
    CSceneObject * CreateSceneObject(const std::string & _name, const CSceneObjectParams & _params);

    CSceneObjectGeometry * GetGeometry(const std::string & _name);
    SceneObjectTemplatesT & GetGeometries();

    bool Load();
    bool Store();

    inline void SetScene(CScene * _scene);
    inline CScene * GetScene();

protected:
private:
    SceneObjectTemplatesT mSceneObjectTemplates;

    CScene * mScene = nullptr;
};

inline void CGeometryManager::SetScene(CScene * _scene)
{
    mScene = _scene;
}

inline CScene *CGeometryManager::GetScene()
{
    return mScene;
}

}// namespace drash

#endif // TEMPLATESYSTEM_H
