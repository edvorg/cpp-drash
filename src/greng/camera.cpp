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

#include "camera.h"
#include "../misc/ray.h"
#include "../misc/plane.h"

namespace greng {

    using drash::CVec4f;

    void CCamera::Step(double _dt) {
        bool compute_matrices = false;

        if (orthoWidthAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (fovAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (depthOfViewAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (posAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (rotationAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (aspectRatioAnimator.Step(_dt)) {
            compute_matrices = true;
        }

        if (compute_matrices == true) {
            ComputeMatrices();
        }
    }

    void CCamera::LookAt(const CVec3f& _point) {
        CVec3f dir = _point;
        dir -= pos;

        CVec2f dirx(-dir.z, -dir.x);
        CVec2f diry(dirx.Length(), dir.y);

        dirx.Normalize();
        diry.Normalize();

        CVec3f rot;

        rot.y = acos(dirx.x);
        rot.x = asin(diry.y);

        if (dirx.y < 0) {
            rot.y = -rot.y;
        }

        rotationAnimator = rot;
    }

    void CCamera::Forward(float _distance) {
        CVec4f z(0, 0, -1, 1);
        CVec4f dir;
        MatrixMultiply(antiRotationMatrix, z, dir);

        dir.Normalize();
        dir.x *= _distance;
        dir.y *= _distance;
        dir.z *= _distance;
        dir.w *= _distance;

        CVec3f new_pos(pos);
        new_pos += dir;
        posAnimator = new_pos;
    }

    void CCamera::Strafe(float _distance) {
        CVec3f up(0, 1, 0);
        CVec4f z(0, 0, -1, 1);
        CVec4f dir;
        MatrixMultiply(antiRotationMatrix, z, dir);

        CVec3f strafe_dir;
        Vec3Cross(up, dir, strafe_dir);

        strafe_dir.x *= _distance;
        strafe_dir.y *= _distance;
        strafe_dir.z *= _distance;

        strafe_dir += pos;
        posAnimator = strafe_dir;
    }

    void CCamera::CastRay(const CVec2f& _pos, const drash::CPlane& _plane,
                          CVec3f& _result) const {
        double c = 1.0 / cos(fov / 2.0); // hypotenuse

        double frame_height = 2.0 * sqrt(c * c - 1.0);
        double frame_width = frame_height * aspectRatio;

        CVec2f ppos = _pos;

        ppos.x *= frame_width;
        ppos.y *= frame_height;

        CVec4f dir(ppos, -1, 1);
        CVec4f tmp;
        MatrixMultiply(antiRotationMatrix, dir, tmp);

        drash::CRay r;
        r.SetPoint(pos);
        r.SetDirection(tmp);
        _plane.CastRay(r, _result);
    }

    void CCamera::ComputeMatrices() {
        CMatrix4f rotx;
        MatrixRotationX(rotx, -rotation.x);

        CMatrix4f roty;
        MatrixRotationY(roty, -rotation.y);

        MatrixMultiply(rotx, roty, rotationMatrix);
        MatrixRotationX(rotx, rotation.x);
        MatrixRotationY(roty, rotation.y);
        MatrixMultiply(roty, rotx, antiRotationMatrix);

        CVec3f tv(-pos.x, -pos.y, -pos.z);
        CMatrix4f tm;
        MatrixTranslation(tm, tv);

        MatrixMultiply(rotationMatrix, tm, viewMatrix);

        if (ortho) {
            projectionMatrix.Identity();
            projectionMatrix.data[projectionMatrix.i00] = 1.0 / orthoWidth;
            projectionMatrix.data[projectionMatrix.i11] =
                aspectRatio / orthoWidth;
            projectionMatrix.data[projectionMatrix.i22] = -1.0 / depthOfView;
        } else {
            Matrix4Perspective(projectionMatrix, fov, aspectRatio, 1.0,
                               depthOfView + 1.0f);
        }

        viewMatrixTransposed = viewMatrix;
        viewMatrixTransposed.Transpose();

        projectionMatrixTransposed = projectionMatrix;
        projectionMatrixTransposed.Transpose();
    }

} // namespace greng
