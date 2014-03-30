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

#include "playerssystem.h"

#include "../players/player.h"
#include "../diag/logger.h"
#include "../scene/scene.h"

namespace drash {

    PlayersSystem::PlayersSystem(Scene& _scene)
        : scene(_scene), playersFactory(playersCountLimit, "Player") {}

    void PlayersSystem::Step(double) {}

    PlayersSystem::~PlayersSystem() {
        while (playersFactory.EnumObjects() != 0) {
            DestroyPlayer(playersFactory.GetObjects()[0]);
        }
    }

    Player* PlayersSystem::CreatePlayer(const SceneObjectGeometry& _g,
                                          const PlayerParams& _p) {
        Player* res = playersFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->sceneObject = scene.CreateObject(_g, _p.sceneObjectParams);

        if (res->sceneObject == nullptr) {
            playersFactory.DestroyObject(res);
            return nullptr;
        }

        res->sceneObject->SetFixedRotation(true);
        res->velocityLimit = _p.velocityLimit;

        return res;
    }

    bool PlayersSystem::DestroyPlayer(Player* _player) {
        if (playersFactory.IsObject(_player) == false) {
            return false;
        }

        if (_player->sceneObject != nullptr) {
            scene.DestroyObject(_player->sceneObject);
        }

        playersFactory.DestroyObject(_player);

        return true;
    }

    bool PlayersSystem::SendMessage(Player* _player,
                                     const PlayerMessage& _message) {
        if (playersFactory.IsObject(_player) == false) {
            LOG_ERR("PlayersSystem::SendMessage(): Wrong player");
            return false;
        }

        switch (_message) {
        case PlayerMessage::Left:
            if (_player->GetSceneObject()->GetLinearVelocity().Length() <
                _player->velocityLimit) {
                _player->GetSceneObject()->ApplyLinearImpulse(
                    Vec2f(-1.0, 0),
                    _player->GetSceneObject()->GetMassCenter());
            }
            break;

        case PlayerMessage::Right:
            if (_player->GetSceneObject()->GetLinearVelocity().Length() <
                _player->velocityLimit) {
                _player->GetSceneObject()->ApplyLinearImpulse(
                    Vec2f(1.0, 0), _player->GetSceneObject()->GetMassCenter());
            }
            break;

        case PlayerMessage::Deep:
            _player->GetSceneObject()->GetPosZ().SetTarget(
                _player->GetSceneObject()->GetPosZ() - 5.0, 0.5,
                AnimatorBehavior::Single);
            break;

        case PlayerMessage::AntiDeep:
            _player->GetSceneObject()->GetPosZ().SetTarget(
                _player->GetSceneObject()->GetPosZ() + 5.0, 0.5,
                AnimatorBehavior::Single);
            break;

        case PlayerMessage::Jump:
            _player->GetSceneObject()->ApplyLinearImpulse(
                Vec2f(0, 0.5), _player->GetSceneObject()->GetMassCenter());
            break;

        default:
            break;
        }

        return true;
    }

} // namespace drash
