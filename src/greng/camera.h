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
#ifndef OBJECTCAMERA_H
#define OBJECTCAMERA_H

#include "../misc/animator.h"
#include "../misc/matrix4.h"
#include "../misc/objectfactory.h"

namespace drash {

    class Plane;
    class Matrix4f;

} // namespace drash

namespace greng {

    using drash::Animator;
    using drash::Vec2f;
    using drash::Vec3f;
    using drash::Matrix4f;

    class Camera : public drash::ObjectFactory<Camera>::FactoryProduct {
    public:
        friend class CameraManager;

        void Step(double _dt);

        void SetOrtho(bool _ortho) { ortho = _ortho; }
        bool IsOrtho() const { return ortho; }

        auto& GetOrthoWidth() { return orthoWidthAnimator; }
        auto& GetOrthoHeight() { return orthoHeightAnimator; }
        auto& GetFov() { return fovAnimator; }
        auto& GetDepthOfView() { return depthOfViewAnimator; }
        auto& GetPos() { return posAnimator; }
        auto& GetPos() const { return posAnimator; }
        auto& GetRotation() { return rotationAnimator; }
        auto& GetAspectRatio() { return aspectRatioAnimator; }
        auto& GetAspectRatio() const { return aspectRatioAnimator; }

        void LookAt(const Vec3f& _point);

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

        void CastRay(const Vec2f& _pos, const drash::Plane& _plane,
                     Vec3f& _result) const;

    protected:
    private:
        void ComputeMatrices();

        bool ortho = false;
        float orthoWidth;
        float orthoHeight;
        float fov;
        float depthOfView;
        Vec3f pos;
        Vec2f rotation;
        Animator<float> orthoWidthAnimator = orthoWidth;
        Animator<float> orthoHeightAnimator = orthoHeight;
        Animator<float> fovAnimator = fov;
        Animator<float> depthOfViewAnimator = depthOfView;
        Animator<Vec3f> posAnimator = pos;
        Animator<Vec2f> rotationAnimator = rotation;

        Matrix4f rotationMatrix;
        Matrix4f antiRotationMatrix;
        Matrix4f viewMatrix;
        Matrix4f projectionMatrix;

        Matrix4f viewMatrixTransposed;
        Matrix4f projectionMatrixTransposed;

        float aspectRatio = 1.0f;
        Animator<float> aspectRatioAnimator = aspectRatio;
    };

} // namespace greng

#endif // OBJECTCAMERA_H
