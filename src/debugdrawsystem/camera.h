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

#include "../misc/animatedparam.h"
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

    inline CAnimatedParam<float> &GetOrthoWidth();
    inline CAnimatedParam<float> &GetFov();
    inline CAnimatedParam<float> &GetDepthOfView();
    inline CAnimatedParam<CVec3f> &GetPos();
    inline CAnimatedParam<CVec2f> &GetRotation();

    void LookAt(const CVec3f &_point);

    void Forward(float _distance);
    void Strafe(float _distance);

    inline const CMatrix4f &GetRotationMatrix() const;
    inline const CMatrix4f &GetViewMatrix() const;

protected:        
    CCamera(void);

    bool Init(const CCameraParams &_params);

    void Step(double _dt);

private:
    void ComputeMatrices();

    bool mOrtho = false;
    CAnimatedParam<float> mOrthoWidth;
    CAnimatedParam<float> mFov;
    CAnimatedParam<float> mDepthOfView;
    CAnimatedParam<CVec3f> mPos;
    CAnimatedParam<CVec2f> mRotation;

    CMatrix4f mRotationMatrix;
    CMatrix4f mViewMatrix;
};

inline void CCamera::SetOrtho(bool _ortho)
{
    mOrtho = _ortho;
}

inline bool CCamera::IsOrtho() const
{
    return mOrtho;
}

inline CAnimatedParam<float> &CCamera::GetOrthoWidth()
{
    return mOrthoWidth;
}

inline CAnimatedParam<float> &CCamera::GetFov()
{
    return mFov;
}

inline CAnimatedParam<float> &CCamera::GetDepthOfView()
{
    return mDepthOfView;
}

inline CAnimatedParam<CVec3f> &CCamera::GetPos()
{
    return mPos;
}

inline CAnimatedParam<CVec2f> &CCamera::GetRotation()
{
    return mRotation;
}

inline const CMatrix4f &CCamera::GetRotationMatrix() const
{
    return mRotationMatrix;
}

inline const CMatrix4f &CCamera::GetViewMatrix() const
{
    return mViewMatrix;
}

}// namespace drash

#endif // COBJECTCAMERA_H