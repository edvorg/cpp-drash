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

#include "leveldesc.h"

using namespace std;

namespace drash {

    LevelDesc::LevelDesc()
        : objectsFactory(objectsCountLimit, "LevelObjectDesc") {}

    LevelDesc::~LevelDesc() { objectsFactory.DestroyObjects(); }

    LevelObjectDesc* LevelDesc::AddObject(const std::string& _geometry,
                                          const std::string& _name) {
        auto o = objectsFactory.CreateObject();

        if (o == nullptr) {
            return nullptr;
        }

        o->geometryName = _geometry;
        o->levelObjectName = _name;

        return o;
    }

    bool LevelDesc::DestroyObject(LevelObjectDesc* _desc) {
        if (!objectsFactory.IsObject(_desc)) {
            LOG_ERR("LevelDesc::DestroyObject(): invalid object taken");
            return false;
        }

        objectsFactory.DestroyObject(_desc);
        return true;
    }

    LevelObjectDesc* LevelDesc::GetObject(const std::string& _name) {
        for (unsigned int i = 0; i < objectsFactory.EnumObjects(); i++) {
            if (objectsFactory.GetObjects()[i]->levelObjectName == _name) {
                return objectsFactory.GetObjects()[i];
            }
        }

        return nullptr;
    }

    void LevelDesc::DestroyObjects() { objectsFactory.DestroyObjects(); }

    std::string LevelDesc::GetUniqueObjectName() const {
        std::string res("object_0");

        for (unsigned int i = 0; i < objectsFactory.EnumObjects(); i++) {
            if (objectsFactory.GetObjects()[i]->levelObjectName == res) {
                res += "0";
            }
        }

        return res;
    }

    bool LevelDesc::Store(const string& _filename) const {
        ofstream out(_filename);

        if (out.is_open() == false) {
            LOG_ERR("Can not load level from file " << _filename.c_str());
            return false;
        }

        out << objectsFactory.EnumObjects() << endl;
        for (unsigned int i = 0; i < objectsFactory.EnumObjects(); i++) {
            LevelObjectDesc* desc = objectsFactory.GetObjects()[i];

            // Save name Template
            out << desc->levelObjectName << endl;
            out << desc->geometryName << endl;

            const SceneObjectParams& cur = desc->params;

            out << cur.angle << endl;
            out << cur.dynamic << endl;
            out << cur.fixedRotation << endl;
            out << cur.pos.x << endl;
            out << cur.pos.y << endl;
            out << cur.pos.z << endl;
        }

        out.close();

        return true;
    }

    bool LevelDesc::Load(const string& _filename) {
        ifstream in(_filename);

        if (in.is_open() == false) {
            LOG_ERR("Can not store level from file " << _filename.c_str());
            return false;
        }

        DestroyObjects();

        int objects_count = 0;
        in >> objects_count;
        for (int i = 0; i < objects_count; i++) {
            string name_object = "";
            std::string name_geometry = "";
            SceneObjectParams params;

            in >> name_object;
            in >> name_geometry;
            in >> params.angle;
            in >> params.dynamic;
            in >> params.fixedRotation;
            in >> params.pos.x;
            in >> params.pos.y;
            in >> params.pos.z;

            LevelObjectDesc* desc = AddObject(name_geometry, name_object);

            if (desc != nullptr) {
                desc->geometryName = name_geometry;
                desc->levelObjectName = name_object;
                desc->params = params;
            }
        }

        in.close();

        return true;
    }

} // namespace drash
