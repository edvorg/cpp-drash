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

#include "level.h"

using namespace std;

namespace drash
{

CSceneObjectParams *CLevel::AddObject(const std::string &_template,
                                      const std::string &_name)
{
    auto template_objects = mObjects.find(_template);

    if (template_objects == mObjects.end())
    {
        template_objects = mObjects.insert(pair<string,
                                           CLevel::LEVEL_NODE>(_template,
                                                               CLevel::LEVEL_NODE())).first;
    }

    auto object = template_objects->second.find(_name);

    if (object != template_objects->second.end())
    {
        LOG_ERR("CLevel::AddObject(): object "<<_name.c_str()<<" already exists");
        return nullptr;
    }

    return &template_objects->second.insert(pair<string,
                                            CSceneObjectParams>(_name,
                                                                CSceneObjectParams())).first->second;
}

bool CLevel::Store(const string &_filename) const
{
    ofstream out(_filename);

    if (out.is_open() == false) {
        LOG_ERR("Can not load level from file " << _filename.c_str());
        return false;
    }

//    map<string, LEVEL_NODE> mObjects;
    out << mObjects.size() << endl;
    for (auto &itemTemplates : mObjects) {
        // Save name Template
        out << itemTemplates.first << endl;
        out << itemTemplates.second.size() << endl;
        for (auto &ObjectItem : itemTemplates.second) {
            out << ObjectItem.first << endl;
            const CSceneObjectParams &cur = ObjectItem.second;
            out << cur.mAngle << endl;
            out << cur.mDynamic << endl;
            out << cur.mFixedRotation << endl;
            out << cur.mPos.mX << endl;
            out << cur.mPos.mY << endl;
            out << cur.mPos.mZ << endl;
        }
    }

    out.close();

    return true;
}

bool CLevel::Load(const string &_filename)
{
    ifstream in(_filename);

    if (in.is_open() == false) {
        LOG_ERR("Can not store level from file " << _filename.c_str());
        return false;
    }

    mObjects.clear();

    int count_templates = 0;
    in >> count_templates;
    for (int i = 0 ; i < count_templates ; i++) {
        std::string name_template = "";
        in >> name_template;
        int count_object = 0;
        in >> count_object;
        LEVEL_NODE buffMap;
        for (int i = 0 ; i < count_object ; i++) {
            string name_object = "";
            CSceneObjectParams params;
            in >> name_object;
            in >> params.mAngle;
            in >> params.mDynamic;
            in >> params.mFixedRotation;
            in >> params.mPos.mX;
            in >> params.mPos.mY;
            in >> params.mPos.mZ;
            pair<string,CSceneObjectParams> p(name_object,params);
            buffMap.insert(p);
        }
        pair<string,LEVEL_NODE> p(name_template,buffMap);
        mObjects.insert(p);
    }

    in.close();

    return true;
}

} // namespace drash
