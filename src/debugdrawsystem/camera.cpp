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

void CCamera::LookAt(const CVec3f &_point)
{
    CVec3f dir = _point;
    dir -= mPos;

    CVec2f dirx(-dir.mZ, -dir.mX);
    CVec2f diry(dirx.Length(), dir.mY);

    dirx.Normalize();
    diry.Normalize();

    CVec3f rot;

    rot.mY = acos(dirx.mX);
    rot.mX = asin(diry.mY);

    if (dirx.mY < 0)
    {
        rot.mY = -rot.mY;
    }

    mRotationAnimator = rot;
}

void CCamera::Forward(float _distance)
{
    CVec4f z(0, 0, -1, 1);
    CVec4f dir;
    MatrixMultiply(mAntiRotationMatrix, z, dir);

    dir.Normalize();
    dir.mX *= _distance;
    dir.mY *= _distance;
    dir.mZ *= _distance;
    dir.mW *= _distance;

    CVec3f new_pos(mPos);
    new_pos += dir;
    mPosAnimator = new_pos;
}

void CCamera::Strafe(float _distance)
{
    CVec3f up(0, 1, 0);
    CVec4f z(0, 0, -1, 1);
    CVec4f dir;
    MatrixMultiply(mAntiRotationMatrix, z, dir);

    CVec3f strafe_dir;
    Vec3Cross(up, dir, strafe_dir);

    strafe_dir.mX *= _distance;
    strafe_dir.mY *= _distance;
    strafe_dir.mZ *= _distance;

    strafe_dir += mPos;
    mPosAnimator = strafe_dir;
}

void CCamera::SetAspectRatio(float _aspect)
{
    if (math::Abs(_aspect) < 0.000001)
    {
        _aspect = 1.0f;
    }

    mAspectRatio = _aspect;

    ComputeMatrices();
}

bool CCamera::Init(const CCameraParams &_params)
{
    mOrthoWidthAnimator = _params.mOrthoWidth;
    mFovAnimator = _params.mFov;
    mDepthOfViewAnimator = _params.mDepthOfView;
    mPosAnimator = _params.mPos;
    mRotationAnimator = _params.mRotation;

    return true;
}

void CCamera::Step(double _dt)
{
    bool compute_matrices = false;

    if (mOrthoWidthAnimator.Step(_dt))
    {
        compute_matrices = true;
    }

    if (mFovAnimator.Step(_dt))
    {
        compute_matrices = true;
    }

    if (mDepthOfViewAnimator.Step(_dt))
    {
        compute_matrices = true;
    }

    if (mPosAnimator.Step(_dt))
    {
        compute_matrices = true;
    }

    if (mRotationAnimator.Step(_dt))
    {
        compute_matrices = true;
    }

    if (compute_matrices == true)
    {
        ComputeMatrices();
    }
}

void CCamera::ComputeMatrices()
{
    CMatrix4f rotx;
    MatrixRotationX(rotx, -mRotation.mX);

    CMatrix4f roty;
    MatrixRotationY(roty, -mRotation.mY);

    MatrixMultiply(rotx, roty, mRotationMatrix);
    MatrixRotationX(rotx, mRotation.mX);
    MatrixRotationY(roty, mRotation.mY);
    MatrixMultiply(roty, rotx, mAntiRotationMatrix);

    CVec3f tv(-mPos.mX, -mPos.mY, -mPos.mZ);
    CMatrix4f tm;
    MatrixTranslation(tm, tv);

    MatrixMultiply(mRotationMatrix, tm, mViewMatrix);

    Matrix4Perspective(mProjectionMatrix, mFov, mAspectRatio, 1.0, mDepthOfView + 1.0f);
}

}// namespace drash
