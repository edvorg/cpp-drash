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

    CPlayersSystem::CPlayersSystem(CScene& _scene)
        : mScene(_scene), mPlayersFactory(mPlayersCountLimit, "CPlayer") {}

    void CPlayersSystem::Step(double) {}

    CPlayersSystem::~CPlayersSystem() {
        while (mPlayersFactory.EnumObjects() != 0) {
            DestroyPlayer(mPlayersFactory.GetObjects()[0]);
        }
    }

    CPlayer* CPlayersSystem::CreatePlayer(const CSceneObjectGeometry& _g,
                                          const CPlayerParams& _p) {
        CPlayer* res = mPlayersFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->mSceneObject = mScene.CreateObject(_g, _p.mSceneObjectParams);

        if (res->mSceneObject == nullptr) {
            mPlayersFactory.DestroyObject(res);
            return nullptr;
        }

        res->mSceneObject->SetFixedRotation(true);
        res->mVelocityLimit = _p.mVelocityLimit;

        return res;
    }

    bool CPlayersSystem::DestroyPlayer(CPlayer* _player) {
        if (mPlayersFactory.IsObject(_player) == false) {
            return false;
        }

        if (_player->mSceneObject != nullptr) {
            mScene.DestroyObject(_player->mSceneObject);
        }

        mPlayersFactory.DestroyObject(_player);

        return true;
    }

    bool CPlayersSystem::SendMessage(CPlayer* _player,
                                     const PlayerMessage& _message) {
        if (mPlayersFactory.IsObject(_player) == false) {
            LOG_ERR("CPlayersSystem::SendMessage(): Wrong player");
            return false;
        }

        switch (_message) {
        case PlayerMessage::Left:
            if (_player->GetSceneObject()->GetLinearVelocity().Length() <
                _player->mVelocityLimit) {
                _player->GetSceneObject()->ApplyLinearImpulse(
                    CVec2f(-1.0, 0),
                    _player->GetSceneObject()->GetMassCenter());
            }
            break;

        case PlayerMessage::Right:
            if (_player->GetSceneObject()->GetLinearVelocity().Length() <
                _player->mVelocityLimit) {
                _player->GetSceneObject()->ApplyLinearImpulse(
                    CVec2f(1.0, 0), _player->GetSceneObject()->GetMassCenter());
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
                CVec2f(0, 0.5), _player->GetSceneObject()->GetMassCenter());
            break;

        default:
            break;
        }

        return true;
    }

} // namespace drash