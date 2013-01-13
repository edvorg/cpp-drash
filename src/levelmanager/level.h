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

#ifndef DRASH_LEVEL_H
#define DRASH_LEVEL_H

#include "../scene/sceneobject.h"

namespace drash
{

using std::map;
using std::string;
using std::vector;
using std::pair;

class CLevel : public CObjectFactory<CLevel>::CFactoryProduct
{
public:
    friend class CLevelManager;

    typedef map<string, CSceneObjectParams> LEVEL_NODE;

    CLevel() = default;

    CSceneObjectParams *AddObject(const std::string &_template,
                                  const std::string &_name);

    bool Store(const string &_filename)const;

    bool Load(const string &_filename);

    inline map<string,LEVEL_NODE> GetObjects()const;

private:
    map<string, LEVEL_NODE> mObjects;
};

inline map<string,CLevel::LEVEL_NODE> CLevel::GetObjects() const {
    return mObjects;
}

} // namespace drash

#endif // DRASH_LEVEL_H
