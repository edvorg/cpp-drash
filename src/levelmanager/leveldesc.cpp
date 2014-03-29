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

namespace drash
{

CLevelDesc::CLevelDesc():
    mObjectsFactory(mObjectsCountLimit, "CLevelObjectDesc")
{
}

CLevelDesc::~CLevelDesc()
{
    mObjectsFactory.DestroyObjects();
}

CLevelObjectDesc * CLevelDesc::AddObject(const std::string & _geometry,
                                         const std::string & _name)
{
    auto o = mObjectsFactory.CreateObject();

    if (o == nullptr)
    {
        return nullptr;
    }

    o->mGeometryName = _geometry;
    o->mLevelObjectName = _name;

    return o;
}

bool CLevelDesc::DestroyObject(CLevelObjectDesc *_desc)
{
    if (!mObjectsFactory.IsObject(_desc))
    {
        LOG_ERR("CLevelDesc::DestroyObject(): invalid object taken");
        return false;
    }

    mObjectsFactory.DestroyObject(_desc);
    return true;
}

CLevelObjectDesc *CLevelDesc::GetObject(const std::string &_name)
{
    for (unsigned int i = 0; i < mObjectsFactory.EnumObjects(); i++)
    {
        if (mObjectsFactory.GetObjects()[i]->mLevelObjectName == _name)
        {
            return mObjectsFactory.GetObjects()[i];
        }
    }

    return nullptr;
}

void CLevelDesc::DestroyObjects()
{
    mObjectsFactory.DestroyObjects();
}

std::string CLevelDesc::GetUniqueObjectName() const
{
    std::string res("object_0");

    for (unsigned int i = 0; i < mObjectsFactory.EnumObjects(); i++)
    {
        if (mObjectsFactory.GetObjects()[i]->mLevelObjectName == res)
        {
            res += "0";
        }
    }

    return res;
}

bool CLevelDesc::Store(const string & _filename) const
{
    ofstream out(_filename);

    if (out.is_open() == false)
    {
        LOG_ERR("Can not load level from file " << _filename.c_str());
        return false;
    }

    out << mObjectsFactory.EnumObjects() << endl;
    for (unsigned int i = 0; i < mObjectsFactory.EnumObjects(); i++)
    {
        CLevelObjectDesc * desc = mObjectsFactory.GetObjects()[i];

        // Save name Template
        out << desc->mLevelObjectName << endl;
        out << desc->mGeometryName << endl;

        const CSceneObjectParams &cur = desc->mParams;

        out << cur.mAngle << endl;
        out << cur.mDynamic << endl;
        out << cur.mFixedRotation << endl;
        out << cur.mPos.mX << endl;
        out << cur.mPos.mY << endl;
        out << cur.mPos.mZ << endl;
    }

    out.close();

    return true;
}

bool CLevelDesc::Load(const string & _filename)
{
    ifstream in(_filename);

    if (in.is_open() == false)
    {
        LOG_ERR("Can not store level from file " << _filename.c_str());
        return false;
    }

    DestroyObjects();

    int objects_count = 0;
    in >> objects_count;
    for (int i = 0 ; i < objects_count ; i++)
    {
        string name_object = "";
        std::string name_geometry = "";
        CSceneObjectParams params;

        in >> name_object;
        in >> name_geometry;
        in >> params.mAngle;
        in >> params.mDynamic;
        in >> params.mFixedRotation;
        in >> params.mPos.mX;
        in >> params.mPos.mY;
        in >> params.mPos.mZ;

        CLevelObjectDesc * desc = AddObject(name_geometry, name_object);

        if (desc != nullptr)
        {
            desc->mGeometryName = name_geometry;
            desc->mLevelObjectName = name_object;
            desc->mParams = params;
        }
    }

    in.close();

    return true;
}

} // namespace drash
