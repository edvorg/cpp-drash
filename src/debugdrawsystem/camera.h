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

#pragma once
#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "../misc/animator.h"
#include "../misc/vec3.h"
#include "../misc/matrix4.h"

namespace drash
{

class CCameraParams
{
public:
    bool mOrtho = false;
    float mOrthoWidth = 1.0f;
    float mFov = M_PI / 4.0f;
    float mDepthOfView = 1000.0f;
    CVec3f mPos;
    CVec3f mRotation;
};

class CCamera
{
public:
    friend class CDebugDrawSystem;

    inline void SetOrtho(bool _ortho);
    inline bool IsOrtho() const;

    inline CAnimator<float> &GetOrthoWidth();
    inline CAnimator<float> &GetFov();
    inline CAnimator<float> &GetDepthOfView();
    inline CAnimator<CVec3f> &GetPos();
    inline CAnimator<CVec2f> &GetRotation();

    void LookAt(const CVec3f &_point);

    void Forward(float _distance);
    void Strafe(float _distance);

    void SetAspectRatio(float _aspect);

    inline const CMatrix4f &GetRotationMatrix() const;
    inline const CMatrix4f &GetAntiRotationMatrix() const;
    inline const CMatrix4f &GetViewMatrix() const;
    inline const CMatrix4f &GetProjectionMatrix() const;

protected:        
    CCamera(void) = default;

    bool Init(const CCameraParams &_params);

    void Step(double _dt);

private:
    void ComputeMatrices();

    bool mOrtho = false;
    float mOrthoWidth;
    float mFov;
    float mDepthOfView;
    CVec3f mPos;
    CVec2f mRotation;
    CAnimator<float> mOrthoWidthAnimator = mOrthoWidth;
    CAnimator<float> mFovAnimator = mFov;
    CAnimator<float> mDepthOfViewAnimator = mDepthOfView;
    CAnimator<CVec3f> mPosAnimator = mPos;
    CAnimator<CVec2f> mRotationAnimator = mRotation;

    CMatrix4f mRotationMatrix;
    CMatrix4f mAntiRotationMatrix;
    CMatrix4f mViewMatrix;
    CMatrix4f mProjectionMatrix;

    float mAspectRatio = 1.0f;
};

inline void CCamera::SetOrtho(bool _ortho)
{
    mOrtho = _ortho;
}

inline bool CCamera::IsOrtho() const
{
    return mOrtho;
}

inline CAnimator<float> &CCamera::GetOrthoWidth()
{
    return mOrthoWidthAnimator;
}

inline CAnimator<float> &CCamera::GetFov()
{
    return mFovAnimator;
}

inline CAnimator<float> &CCamera::GetDepthOfView()
{
    return mDepthOfViewAnimator;
}

inline CAnimator<CVec3f> &CCamera::GetPos()
{
    return mPosAnimator;
}

inline CAnimator<CVec2f> &CCamera::GetRotation()
{
    return mRotationAnimator;
}

inline const CMatrix4f &CCamera::GetRotationMatrix() const
{
    return mRotationMatrix;
}

inline const CMatrix4f &CCamera::GetAntiRotationMatrix() const
{
    return mAntiRotationMatrix;
}

inline const CMatrix4f &CCamera::GetViewMatrix() const
{
    return mViewMatrix;
}

inline const CMatrix4f &CCamera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

}// namespace drash

#endif // COBJECTCAMERA_H
