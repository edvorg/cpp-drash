// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

namespace greng
{

using drash::CLogger;

bool CCameraManager::Init()
{
    Release();

    return true;
}

void CCameraManager::Release()
{
    while (mCameraFactory.EnumObjects() != 0)
    {
        DestroyCamera(mCameraFactory.GetObjects()[0]);
    }
}

CCamera *CCameraManager::CreateCamera()
{
    CCamera *res = mCameraFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    return res;
}

bool CCameraManager::DestroyCamera(CCamera *_camera)
{
    if (mCameraFactory.IsObject(_camera) == false)
    {
        LOG_ERR("CCameraManager::DestroyCamera(): Invalid object taken");
        return false;
    }

    mCameraFactory.DestroyObject(_camera);

    return true;
}

} // namespace greng
