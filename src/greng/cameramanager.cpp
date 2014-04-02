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

#include "cameramanager.h"
#include "camera.h"

#include "../diag/logger.h"

namespace greng {

    using drash::Logger;

    CameraManager::CameraManager()
        : cameraFactory(camerasCountLimit, "Camera") {}

    CameraManager::~CameraManager() {
        while (cameraFactory.EnumObjects() != 0) {
            DestroyCamera(cameraFactory.GetObjects()[0]);
        }
    }

    void CameraManager::Step(double _dt) {
        for (unsigned int i = 0; i < cameraFactory.EnumObjects(); i++) {
            cameraFactory.GetObjects()[i]->Step(_dt);
        }
    }

    Camera* CameraManager::CreateCamera(const CameraParams& _params) {
        Camera* res = cameraFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->aspectRatio = aspectRatio;
        res->orthoSize = _params.orthoSize;
        res->fov = _params.fov;
        res->depthOfView = _params.depthOfView;
        res->pos = _params.pos;
        res->rotation = _params.rotation;

        return res;
    }

    bool CameraManager::DestroyCamera(Camera* _camera) {
        if (cameraFactory.IsObject(_camera) == false) {
            LOG_ERR("CameraManager::DestroyCamera(): Invalid object taken");
            return false;
        }

        cameraFactory.DestroyObject(_camera);

        return true;
    }

    void CameraManager::SetAspectRatio(float _ratio) {
        if (drash::math::Abs(_ratio) < 0.000001) {
            _ratio = 1.0f;
        }

        aspectRatio = _ratio;

        for (unsigned int i = 0; i < cameraFactory.EnumObjects(); i++) {
            cameraFactory.GetObjects()[i]->GetAspectRatio() = _ratio;
        }
    }

} // namespace greng
