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

namespace greng
{

using drash::CLogger;

CCameraManager::CCameraManager():
    mCameraFactory(mCamerasCountLimit, "CCamera")
{
}

CCameraManager::~CCameraManager()
{
}

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

void CCameraManager::Step(double _dt)
{
    for (unsigned int i = 0; i < mCameraFactory.EnumObjects(); i++)
    {
        mCameraFactory.GetObjects()[i]->Step(_dt);
    }
}

CCamera *CCameraManager::CreateCamera(const CCameraParams &_params)
{
    CCamera *res = mCameraFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    res->mAspectRatioAnimator = mAspectRatio;
    res->mOrthoWidthAnimator = _params.mOrthoWidth;
    res->mFovAnimator = _params.mFov;
    res->mDepthOfViewAnimator = _params.mDepthOfView;
    res->mPosAnimator = _params.mPos;
    res->mRotationAnimator = _params.mRotation;

    res->ComputeMatrices();

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

void CCameraManager::SetAspectRatio(float _ratio)
{
    if (drash::math::Abs(_ratio) < 0.000001)
    {
        _ratio = 1.0f;
    }

    mAspectRatio = _ratio;

    for (unsigned int i = 0; i < mCameraFactory.EnumObjects(); i++)
    {
        mCameraFactory.GetObjects()[i]->GetAspectRatio() = _ratio;
    }
}

} // namespace greng
