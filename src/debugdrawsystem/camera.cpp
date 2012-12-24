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

#include "camera.h"

namespace drash
{

CCamera::CCamera():
    mRotation([this] (const CVec2f &_new_rotation)
    {
    })
{
}

bool CCamera::Init(const CCameraParams &_params)
{
    mOrthoWidth.Set(_params.mOrthoWidth);
    mFov.Set(_params.mFov);
    mDepthOfView.Set(_params.mDepthOfView);
    mPos.Set(_params.mPos);
    mRotation.Set(_params.mRotation);

    return true;
}

void CCamera::Step(double _dt)
{
    mOrthoWidth.Step(_dt);
    mFov.Step(_dt);
    mDepthOfView.Step(_dt);
    mPos.Step(_dt);
    mRotation.Step(_dt);
}

}// namespace drash
