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

#pragma once
#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "../misc/animator.h"
#include "../misc/matrix4.h"
#include "../misc/objectfactory.h"

namespace drash {

    class CPlane;
    class CMatrix4f;

} // namespace drash

namespace greng {

    using drash::CAnimator;
    using drash::CVec2f;
    using drash::CVec3f;
    using drash::CMatrix4f;

    class CCamera : public drash::CObjectFactory<CCamera>::CFactoryProduct {
      public:
        friend class CCameraManager;

        void Step(double _dt);

        inline void SetOrtho(bool _ortho);
        inline bool IsOrtho() const;

        inline CAnimator<float>& GetOrthoWidth();
        inline CAnimator<float>& GetFov();
        inline CAnimator<float>& GetDepthOfView();
        inline CAnimator<CVec3f>& GetPos();
        inline const CAnimator<CVec3f>& GetPos() const;
        inline CAnimator<CVec2f>& GetRotation();
        inline CAnimator<float>& GetAspectRatio();

        void LookAt(const CVec3f& _point);

        void Forward(float _distance);
        void Strafe(float _distance);

        inline const CMatrix4f& GetRotationMatrix() const;
        inline const CMatrix4f& GetAntiRotationMatrix() const;
        inline const CMatrix4f& GetViewMatrix() const;
        inline const CMatrix4f& GetViewMatrixTransposed() const;
        inline const CMatrix4f& GetProjectionMatrix() const;
        inline const CMatrix4f& GetProjectionMatrixTransposed() const;

        void CastRay(const CVec2f& _pos, const drash::CPlane& _plane,
                     CVec3f& _result) const;

      protected:
      private:
        void ComputeMatrices();

        bool ortho = false;
        float orthoWidth;
        float fov;
        float depthOfView;
        CVec3f pos;
        CVec2f rotation;
        CAnimator<float> orthoWidthAnimator = orthoWidth;
        CAnimator<float> fovAnimator = fov;
        CAnimator<float> depthOfViewAnimator = depthOfView;
        CAnimator<CVec3f> posAnimator = pos;
        CAnimator<CVec2f> rotationAnimator = rotation;

        CMatrix4f rotationMatrix;
        CMatrix4f antiRotationMatrix;
        CMatrix4f viewMatrix;
        CMatrix4f projectionMatrix;

        CMatrix4f viewMatrixTransposed;
        CMatrix4f projectionMatrixTransposed;

        float aspectRatio = 1.0f;
        CAnimator<float> aspectRatioAnimator = aspectRatio;
    };

    inline void CCamera::SetOrtho(bool _ortho) { ortho = _ortho; }

    inline bool CCamera::IsOrtho() const { return ortho; }

    inline CAnimator<float>& CCamera::GetOrthoWidth() {
        return orthoWidthAnimator;
    }

    inline CAnimator<float>& CCamera::GetFov() { return fovAnimator; }

    inline CAnimator<float>& CCamera::GetDepthOfView() {
        return depthOfViewAnimator;
    }

    inline CAnimator<CVec3f>& CCamera::GetPos() { return posAnimator; }

    inline const CAnimator<CVec3f>& CCamera::GetPos() const {
        return posAnimator;
    }

    inline CAnimator<CVec2f>& CCamera::GetRotation() {
        return rotationAnimator;
    }

    inline CAnimator<float>& CCamera::GetAspectRatio() {
        return aspectRatioAnimator;
    }

    inline const CMatrix4f& CCamera::GetRotationMatrix() const {
        return rotationMatrix;
    }

    inline const CMatrix4f& CCamera::GetAntiRotationMatrix() const {
        return antiRotationMatrix;
    }

    inline const CMatrix4f& CCamera::GetViewMatrix() const {
        return viewMatrix;
    }

    inline const CMatrix4f& CCamera::GetViewMatrixTransposed() const {
        return viewMatrixTransposed;
    }

    inline const CMatrix4f& CCamera::GetProjectionMatrix() const {
        return projectionMatrix;
    }

    inline const CMatrix4f& CCamera::GetProjectionMatrixTransposed() const {
        return projectionMatrixTransposed;
    }

} // namespace greng

#endif // COBJECTCAMERA_H
