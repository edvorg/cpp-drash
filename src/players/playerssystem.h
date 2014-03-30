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

#ifndef PLAYERSSYSTEM_H
#define PLAYERSSYSTEM_H

#include "../misc/objectfactory.h"

namespace drash {

    class Scene;
    class SceneObjectGeometry;
    class Player;
    class PlayerParams;

    enum class PlayerMessage : unsigned int {
        Left,
        Right,
        Deep,
        AntiDeep,
        Jump,
    };

    class PlayersSystem final {
    public:
        static const unsigned int playersCountLimit = 2;

        PlayersSystem(Scene& _scene);
        ~PlayersSystem();

        void Step(double);

        Player* CreatePlayer(const SceneObjectGeometry& _g,
                              const PlayerParams& _p);
        bool DestroyPlayer(Player* _player);

        inline Player* const* GetPlayers();
        inline unsigned int EnumPlayers() const;

        bool SendMessage(Player* _player, const PlayerMessage& _message);

    private:
        Scene& scene;
        ObjectFactory<Player> playersFactory;
    };

    inline Player* const* PlayersSystem::GetPlayers() {
        return playersFactory.GetObjects();
    }

    inline unsigned int PlayersSystem::EnumPlayers() const {
        return playersFactory.EnumObjects();
    }

} // namespace drash

#endif // PLAYERSSYSTEM_H
