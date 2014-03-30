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

#ifndef DRASH_LEVEL_H
#define DRASH_LEVEL_H

#include "../scene/sceneobject.h"

namespace drash {

    using std::map;
    using std::string;
    using std::vector;
    using std::pair;

    class LevelObjectDesc
        : public ObjectFactory<LevelObjectDesc>::FactoryProduct {
    public:
        std::string geometryName = "";
        std::string levelObjectName = "";
        SceneObjectParams params;
    };

    class LevelDesc : public ObjectFactory<LevelDesc>::FactoryProduct {
    public:
        friend class LevelManager;

        static const unsigned int objectsCountLimit = 128;

        LevelDesc();
        ~LevelDesc();

        LevelObjectDesc* AddObject(const std::string& _geometry,
                                   const std::string& _name);
        bool DestroyObject(LevelObjectDesc* _desc);
        LevelObjectDesc* GetObject(const std::string& _name);
        inline unsigned int EnumObjects() const;
        inline LevelObjectDesc* const* GetObjects() const;
        void DestroyObjects();

        std::string GetUniqueObjectName() const;

        bool Store(const string& _filename) const;
        bool Load(const string& _filename);

    private:
        ObjectFactory<LevelObjectDesc> objectsFactory;
    };

    inline unsigned int drash::LevelDesc::EnumObjects() const {
        return objectsFactory.EnumObjects();
    }

    inline LevelObjectDesc* const* LevelDesc::GetObjects() const {
        return objectsFactory.GetObjects();
    }

} // namespace drash

#endif // DRASH_LEVEL_H
