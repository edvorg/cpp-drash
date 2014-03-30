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

        void SetOrtho(bool _ortho) { ortho = _ortho; }
        bool IsOrtho() const { return ortho; }

        auto& GetOrthoWidth() { return orthoWidthAnimator; }
        auto& GetFov() { return fovAnimator; }
        auto& GetDepthOfView() { return depthOfViewAnimator; }
        auto& GetPos() { return posAnimator; }
        auto& GetPos() const { return posAnimator; }
        auto& GetRotation() { return rotationAnimator; }
        auto& GetAspectRatio() { return aspectRatioAnimator; }
        auto& GetAspectRatio() const { return aspectRatioAnimator; }

        void LookAt(const CVec3f& _point);

        void Forward(float _distance);
        void Strafe(float _distance);

        auto& GetRotationMatrix() const { return rotationMatrix; }
        auto& GetAntiRotationMatrix() const { return antiRotationMatrix; }
        auto& GetViewMatrix() const { return viewMatrix; }
        auto& GetProjectionMatrix() const { return projectionMatrix; }
        auto& GetViewMatrixTransposed() const { return viewMatrixTransposed; }
        auto& GetProjectionMatrixTransposed() const {
            return projectionMatrixTransposed;
        }

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

} // namespace greng

#endif // COBJECTCAMERA_H
