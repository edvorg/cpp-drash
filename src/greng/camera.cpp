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

    using drash::Vec4f;

    Camera::Camera() {
        using react::link;
        
        link(rotation).reconnect(
            rotationMatrix, [] (auto r) {
                Matrix4f rotx;
                Matrix4f roty;
                Matrix4f res;
                MatrixRotationX(rotx, -r.x);
                MatrixRotationY(roty, -r.y);
                MatrixMultiply(rotx, roty, res);
                return res;
            });
        
        link(rotation).reconnect(
            antiRotationMatrix, [] (auto r) {
                Matrix4f rotx;
                Matrix4f roty;
                Matrix4f res;
                MatrixRotationX(rotx, r.x);
                MatrixRotationY(roty, r.y);
                MatrixMultiply(roty, rotx, res);
                return res;
            });

        link(pos, rotationMatrix).reconnect(
            viewMatrix, [] (auto p, auto r) {
                Matrix4f res;
                Vec3f tv(-p.x, -p.y, -p.z);
                Matrix4f tm;
                MatrixTranslation(tm, tv);
                MatrixMultiply(r, tm, res);
                return res;
            });

        link(ortho, orthoSize, depthOfView, aspectRatio, fov).reconnect(
            projectionMatrix, [] (auto o, auto s, auto d, auto a, auto  f) {
                Matrix4f res;
                auto ds = d ? d : 1.0f;
                res.Identity();
                if (o) {
                    res.data[res.i00] = 1.0f / (s.x ? s.x : 1.0f);
                    res.data[res.i11] = 1.0f / (s.y ? s.y : 1.0f);
                    res.data[res.i22] = -1.0 / ds;
                } else {
                    Matrix4Perspective(res, f, a, 1.0, ds + 1.0f);
                }
                return res;
            });

        link(viewMatrix).reconnect(
            viewMatrixTransposed, [] (auto m) {
                Matrix4f res = m;
                res.Transpose();
                return res;
            });

        link(projectionMatrix).reconnect(
            projectionMatrixTransposed, [] (auto m) {
                Matrix4f res = m;
                res.Transpose();
                return res;
            });
    }

    void Camera::Step(double _dt) {
        orthoSizeAnimator.Step(_dt);
        fovAnimator.Step(_dt);
        depthOfViewAnimator.Step(_dt);
        posAnimator.Step(_dt);
        rotationAnimator.Step(_dt);
        aspectRatioAnimator.Step(_dt);
    }

    void Camera::LookAt(const Vec3f& _point) {
        Vec3f dir = _point;
        dir -= pos;

        Vec2f dirx(-dir.z, -dir.x);
        Vec2f diry(dirx.Length(), dir.y);

        dirx.Normalize();
        diry.Normalize();

        Vec3f rot;

        rot.y = acos(dirx.x);
        rot.x = asin(diry.y);

        if (dirx.y < 0) {
            rot.y = -rot.y;
        }

        rotationAnimator = rot;
    }

    void Camera::Forward(float _distance) {
        Vec4f z(0, 0, -1, 1);
        Vec4f dir;
        MatrixMultiply(antiRotationMatrix, z, dir);

        dir.Normalize();
        dir.x *= _distance;
        dir.y *= _distance;
        dir.z *= _distance;
        dir.w *= _distance;

        Vec3f new_pos(pos);
        new_pos += dir;
        posAnimator = new_pos;
    }

    void Camera::Strafe(float _distance) {
        Vec3f up(0, 1, 0);
        Vec4f z(0, 0, -1, 1);
        Vec4f dir;
        MatrixMultiply(antiRotationMatrix, z, dir);

        Vec3f strafe_dir;
        Vec3Cross(up, dir, strafe_dir);

        strafe_dir.x *= _distance;
        strafe_dir.y *= _distance;
        strafe_dir.z *= _distance;

        strafe_dir += pos;
        posAnimator = strafe_dir;
    }

    void Camera::CastRay(const Vec2f& _pos, const drash::Plane& _plane,
                         Vec3f& _result) const {
        double c = 1.0 / cos(fov / 2.0); // hypotenuse

        double frame_height = 2.0 * sqrt(c * c - 1.0);
        double frame_width = frame_height * aspectRatio;

        Vec2f ppos = _pos;

        ppos.x *= frame_width;
        ppos.y *= frame_height;

        Vec4f dir(ppos, -1, 1);
        Vec4f tmp;
        MatrixMultiply(antiRotationMatrix, dir, tmp);

        drash::Ray r;
        r.SetPoint(pos);
        r.SetDirection(tmp);
        _plane.CastRay(r, _result);
    }

} // namespace greng
