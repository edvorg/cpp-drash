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

#include "../greng/renderer.h"
#include "../greng/camera.h"
#include "../misc/rotationablepoint.h"
#include "plane.h"

namespace drash {

    CRotationablePoint::CRotationablePoint(greng::CRenderer& _renderer,
                                           greng::CCamera& _camera)
        : renderer(_renderer), camera(_camera) {}

    void CRotationablePoint::Step(double) {
        CMatrix4f rotx;
        MatrixRotationX(rotx, rotation.x);

        CMatrix4f rotz;
        MatrixRotationZ(rotz, rotation.z);

        CMatrix4f roty;
        MatrixRotationY(roty, rotation.y);

        CMatrix4f rot_yz;
        MatrixMultiply(roty, rotz, rot_yz);

        CMatrix4f rot;
        MatrixMultiply(rotx, rot_yz, rot);

        CVec4f xy_normal(0, 0, 1, 0);
        CVec4f xy_normal_transposed;
        MatrixMultiply(rot, xy_normal, xy_normal_transposed);

        CVec4f xz_normal(0, 1, 0, 0);
        CVec4f xz_normal_transposed;
        MatrixMultiply(rot, xz_normal, xz_normal_transposed);

        CVec4f yz_normal(1, 0, 0, 0);
        CVec4f yz_normal_transposed;
        MatrixMultiply(rot, yz_normal, yz_normal_transposed);

        CPlane xy;
        xy.SetPoint(point);
        xy.SetNormal(xy_normal_transposed);

        CPlane xz;
        xz.SetPoint(point);
        xz.SetNormal(xz_normal_transposed);

        CPlane yz;
        yz.SetPoint(point);
        yz.SetNormal(yz_normal_transposed);

        CVec3f xy_proj;
        camera.CastRay(cursorPos, xy, xy_proj);

        CVec3f xz_proj;
        camera.CastRay(cursorPos, xz, xz_proj);

        CVec3f yz_proj;
        camera.CastRay(cursorPos, yz, yz_proj);

        xy_proj -= point;
        xz_proj -= point;
        yz_proj -= point;

        if (axisRotating == 0) {
            float len_xy = xy_proj.Length();
            float len_xz = xz_proj.Length();
            float len_yz = yz_proj.Length();

            if (radius - 0.1 < len_xy && len_xy < radius + 0.1) {
                axisOvered = 3;
                return;
            } else if (radius - 0.1 < len_xz && len_xz < radius + 0.1) {
                axisOvered = 2;
                return;
            } else if (radius - 0.1 < len_yz && len_yz < radius + 0.1) {
                axisOvered = 1;
                return;
            }

            axisOvered = 0;
        } else {
            if (axisRotating == 3) {
                xy_proj.Normalize();

                rotation.z = acos(xy_proj.x);
                rotation.z = (xy_proj.y > 0.000001f ? rotation.z : -rotation.z);
                rotation.z -= rotationDelta.z;
            } else if (axisRotating == 2) {
                xz_proj.Normalize();

                rotation.y = acos(xz_proj.z);
                rotation.y = (xz_proj.x > 0.000001f ? rotation.y : -rotation.y);
                rotation.y -= rotationDelta.y;
            } else if (axisRotating == 1) {
                yz_proj.Normalize();

                rotation.x = acos(yz_proj.z);
                rotation.x =
                    (yz_proj.y < -0.000001f ? rotation.x : -rotation.x);
                rotation.x -= rotationDelta.x;
            }
        }
    }

    void CRotationablePoint::Render() {
        unsigned int segments = 32;

        float angle = 0;
        float angle_delta = 2.0 * M_PI / segments;

        CMatrix4f rotx;
        MatrixRotationX(rotx, rotation.x);

        CMatrix4f rotz;
        MatrixRotationZ(rotz, rotation.z);

        CMatrix4f roty;
        MatrixRotationY(roty, rotation.y);

        CMatrix4f rot_yz;
        MatrixMultiply(roty, rotz, rot_yz);

        CMatrix4f rot;
        MatrixMultiply(rotx, rot_yz, rot);

        // Draw lines
        CVec4f x(1, 0, 0, 0);

        CVec4f x_transposed;
        MatrixMultiply(rot, x, x_transposed);

        x_transposed.Vec3().Normalize();
        x_transposed.Vec3() *= radius;
        x_transposed.Vec3() += point;

        renderer.DrawLine(camera, point, x_transposed, 1, CColor4f(1, 0, 0, 1),
                          false);

        CVec4f z(0, 0, 1, 0);

        CVec4f z_transposed;
        MatrixMultiply(rot, z, z_transposed);

        z_transposed.Vec3().Normalize();
        z_transposed.Vec3() *= radius;
        z_transposed.Vec3() += point;

        renderer.DrawLine(camera, point, z_transposed, 1, CColor4f(0, 0, 1, 1),
                          false);

        CVec4f y(0, 1, 0, 0);

        CVec4f y_transposed;
        MatrixMultiply(rot, y, y_transposed);

        y_transposed.Vec3().Normalize();
        y_transposed.Vec3() *= radius;
        y_transposed.Vec3() += point;

        renderer.DrawLine(camera, point, y_transposed, 1, CColor4f(0, 1, 0, 1),
                          false);

        if (axisOX == true) {
            for (unsigned int i = 0; i < segments; i++) {
                CVec4f p1(0, radius * sin(angle), -radius * cos(angle), 0);
                CVec4f p2(0, radius * sin(angle + angle_delta),
                          -radius * cos(angle + angle_delta), 0);

                CVec4f p1_transposed;
                CVec4f p2_transposed;

                MatrixMultiply(rot, p1, p1_transposed);
                MatrixMultiply(rot, p2, p2_transposed);

                p1_transposed.Vec3() += point;
                p2_transposed.Vec3() += point;

                renderer.DrawLine(camera, p1_transposed, p2_transposed, 1,
                                  CColor4f(1, 0, 0, axisOvered == 1 ? 0.5 : 1),
                                  false);

                angle += angle_delta;
            }
        }

        ///
        if (axisOZ == true) {
            for (unsigned int i = 0; i < segments; i++) {
                CVec4f p1(radius * cos(angle), radius * sin(angle), 0, 0);
                CVec4f p2(radius * cos(angle + angle_delta),
                          radius * sin(angle + angle_delta), 0, 0);

                CVec4f p1_transposed;
                CVec4f p2_transposed;

                MatrixMultiply(rot, p1, p1_transposed);
                MatrixMultiply(rot, p2, p2_transposed);

                p1_transposed.Vec3() += point;
                p2_transposed.Vec3() += point;

                renderer.DrawLine(camera, p1_transposed, p2_transposed, 1,
                                  CColor4f(0, 0, 1, axisOvered == 3 ? 0.5 : 1),
                                  false);

                angle += angle_delta;
            }
        }

        ///
        if (axisOY == true) {
            for (unsigned int i = 0; i < segments; i++) {
                CVec4f p1(radius * cos(angle), 0, -radius * sin(angle), 0);
                CVec4f p2(radius * cos(angle + angle_delta), 0,
                          -radius * sin(angle + angle_delta), 0);

                CVec4f p1_transposed;
                CVec4f p2_transposed;

                MatrixMultiply(rot, p1, p1_transposed);
                MatrixMultiply(rot, p2, p2_transposed);

                p1_transposed.Vec3() += point;
                p2_transposed.Vec3() += point;

                renderer.DrawLine(camera, p1_transposed, p2_transposed, 1,
                                  CColor4f(0, 1, 0, axisOvered == 2 ? 0.5 : 1),
                                  false);

                angle += angle_delta;
            }
        }
    }

    void CRotationablePoint::RotateBegin() {
        if ((axisOvered == 1 && axisOX == true) ||
            (axisOvered == 2 && axisOY == true) ||
            (axisOvered == 3 && axisOZ == true)) {

            axisRotating = axisOvered;

            CMatrix4f rotx;
            MatrixRotationX(rotx, rotation.x);

            CMatrix4f rotz;
            MatrixRotationZ(rotz, rotation.z);

            CMatrix4f roty;
            MatrixRotationY(roty, rotation.y);

            CMatrix4f rot_yz;
            MatrixMultiply(roty, rotz, rot_yz);

            CMatrix4f rot;
            MatrixMultiply(rotx, rot_yz, rot);

            CVec4f xy_normal(0, 0, 1, 0);
            CVec4f xy_normal_transposed;
            MatrixMultiply(rot, xy_normal, xy_normal_transposed);

            CVec4f xz_normal(0, 1, 0, 0);
            CVec4f xz_normal_transposed;
            MatrixMultiply(rot, xz_normal, xz_normal_transposed);

            CVec4f yz_normal(1, 0, 0, 0);
            CVec4f yz_normal_transposed;
            MatrixMultiply(rot, yz_normal, yz_normal_transposed);

            CPlane xy;
            xy.SetPoint(point);
            xy.SetNormal(xy_normal_transposed);

            CPlane xz;
            xz.SetPoint(point);
            xz.SetNormal(xz_normal_transposed);

            CPlane yz;
            yz.SetPoint(point);
            yz.SetNormal(yz_normal_transposed);

            CVec3f xy_proj;
            camera.CastRay(cursorPos, xy, xy_proj);

            CVec3f xz_proj;
            camera.CastRay(cursorPos, xz, xz_proj);

            CVec3f yz_proj;
            camera.CastRay(cursorPos, yz, yz_proj);

            xy_proj -= point;
            xz_proj -= point;
            yz_proj -= point;

            rotationDelta = rotation;

            if (axisRotating == 3) {
                xy_proj.Normalize();

                rotationDelta.z = acos(xy_proj.x);
                rotationDelta.z = (xy_proj.y > 0.000001f ? rotationDelta.z
                                                         : -rotationDelta.z);
            } else if (axisRotating == 2) {
                xz_proj.Normalize();

                rotationDelta.y = acos(xz_proj.z);
                rotationDelta.y = (xz_proj.x > 0.000001f ? rotationDelta.y
                                                         : -rotationDelta.y);
            } else if (axisRotating == 1) {
                yz_proj.Normalize();

                rotationDelta.x = acos(yz_proj.z);
                rotationDelta.x = (yz_proj.y < -0.000001f ? rotationDelta.x
                                                          : -rotationDelta.x);
            }

            rotationDelta -= rotation;
        }
    }

    void CRotationablePoint::RotateEnd() { axisRotating = 0; }

} // namespace drash
