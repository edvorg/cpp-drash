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

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "../scene/sceneobject.h"

namespace drash {

    class SceneObject;

    class PlayerParams final {
    public:
        SceneObjectParams sceneObjectParams;
        float velocityLimit = 1;
    };

    class Player final : public ObjectFactory<Player>::FactoryProduct {
    public:
        friend class PlayersSystem;

        inline SceneObject* GetSceneObject();

    protected:
    private:
        SceneObject* sceneObject = nullptr;
        float velocityLimit = 1;
    };

    inline SceneObject* Player::GetSceneObject() { return sceneObject; }

} // namespace drash

#endif // PLAYER_H
