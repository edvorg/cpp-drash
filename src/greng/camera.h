// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

#include "../misc/objectfactory.h"
#include "../misc/animator.h"
#include "../misc/vec3.h"
#include "../misc/matrix4.h"

namespace drash
{

class CPlane;

} // namespace drash

namespace greng
{

using drash::CVec2f;
using drash::CVec3f;
using drash::CMatrix4f;
using drash::CObjectFactory;
using drash::CAnimator;

class CCamera : public CObjectFactory<CCamera>::CFactoryProduct
{
public:
    friend class CCameraManager;

    void Step(double _dt);

    inline void SetOrtho(bool _ortho);
    inline bool IsOrtho() const;

    inline CAnimator<float> &GetOrthoWidth();
    inline CAnimator<float> &GetFov();
    inline CAnimator<float> &GetDepthOfView();
    inline CAnimator<CVec3f> &GetPos();
    inline const CAnimator<CVec3f> &GetPos() const;
    inline CAnimator<CVec2f> &GetRotation();
    inline CAnimator<float> &GetAspectRatio();

    void LookAt(const CVec3f &_point);

    void Forward(float _distance);
    void Strafe(float _distance);

    inline const CMatrix4f &GetRotationMatrix() const;
    inline const CMatrix4f &GetAntiRotationMatrix() const;
    inline const CMatrix4f &GetViewMatrix() const;
    inline const CMatrix4f &GetViewMatrixTransposed() const;
    inline const CMatrix4f &GetProjectionMatrix() const;
    inline const CMatrix4f &GetProjectionMatrixTransposed() const;

    void CastRay(const CVec2f &_pos, const drash::CPlane &_plane, CVec3f &_result) const;

protected:

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

    CMatrix4f mViewMatrixTransposed;
    CMatrix4f mProjectionMatrixTransposed;

    float mAspectRatio = 1.0f;
    CAnimator<float> mAspectRatioAnimator = mAspectRatio;
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

inline const CAnimator<CVec3f> &CCamera::GetPos() const
{
    return mPosAnimator;
}

inline CAnimator<CVec2f> &CCamera::GetRotation()
{
    return mRotationAnimator;
}

inline CAnimator<float> &CCamera::GetAspectRatio()
{
    return mAspectRatioAnimator;
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

inline const CMatrix4f &CCamera::GetViewMatrixTransposed() const
{
    return mViewMatrixTransposed;
}

inline const CMatrix4f &CCamera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

inline const CMatrix4f &CCamera::GetProjectionMatrixTransposed() const
{
    return mProjectionMatrixTransposed;
}

}// namespace greng

#endif // COBJECTCAMERA_H
