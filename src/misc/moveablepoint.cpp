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

#include "moveablepoint.h"
#include "plane.h"

namespace drash {

    MoveablePoint::MoveablePoint() { center.Set(0, 0, 0); }

    MoveablePoint::MoveablePoint(Vec3f _point, greng::Camera* _camera)
        : currentCamera(_camera), center(_point) {}

    void MoveablePoint::SetCenter(const Vec3f& _center) { center = _center; }

    void MoveablePoint::SetCamera(greng::Camera* _camera) {
        currentCamera = _camera;
    }

    void MoveablePoint::Step(double) {
        if (currentCamera == nullptr) {
            return;
        }

        Vec4f normal(0, 0, -1, 0);
        Vec4f normal_transformed;
        MatrixMultiply(currentCamera->GetAntiRotationMatrix(), normal,
                       normal_transformed);

        Plane p;
        p.SetPoint(center);
        p.SetNormal(normal_transformed);

        Vec2f p1(0, 0);
        Vec2f p2(lineSizeScreen, 0);

        Vec3f wp1;
        Vec3f wp2;

        currentCamera->CastRay(p1, p, wp1);
        currentCamera->CastRay(p2, p, wp2);

        wp1 -= wp2;

        lineSizeWorld = wp1.Length();

        x = center;
        y = center;
        z = center;

        x.x += lineSizeWorld;
        y.y += lineSizeWorld;
        z.z += lineSizeWorld;
    }

    void MoveablePoint::Render(greng::Renderer& _render) {
        if (currentCamera == nullptr) {
            return;
        }
        _render.DrawLine(*GetCamera(), center, x, 1,
                         Color4f(1 * axisDrawK.x + 10, 0, 0, 1), false);
        _render.DrawLine(*GetCamera(), center, y, 1,
                         Color4f(0, 1 * axisDrawK.y + 10, 0, 1), false);
        _render.DrawLine(*GetCamera(), center, z, 1,
                         Color4f(0, 0, 1 * axisDrawK.z + 10, 1), false);
    }

    greng::Camera* MoveablePoint::GetCamera() { return currentCamera; }

    void MoveablePoint::ClickBegin() {
        if (axisOver == 0) {
            return;
        }
        if (axisOver == 2) {
            Plane xy(PlaneXY);
            xy.SetPoint(center);
            GetCamera()->CastRay(cursorPos, xy, firstClick);
            axisMoving = axisOver;
            moving = true;
        } else {
            Plane xz(PlaneXZ);
            xz.SetPoint(center);
            GetCamera()->CastRay(cursorPos, xz, firstClick);
            axisMoving = axisOver;
            moving = true;
        }
    }

    void MoveablePoint::ClickPressing() {
        if (axisMoving == 1) {
            Plane xz(PlaneXZ);
            xz.SetPoint(center);
            Vec3f new_pos;
            GetCamera()->CastRay(cursorPos, xz, new_pos);

            center.x += new_pos.x - firstClick.x;
            firstClick = new_pos;
        } else if (axisMoving == 2) {
            Plane xy(PlaneXY);
            xy.SetPoint(center);
            Vec3f new_pos;
            GetCamera()->CastRay(cursorPos, xy, new_pos);

            center.y += new_pos.y - firstClick.y;
            firstClick = new_pos;
        } else if (axisMoving == 3) {
            Plane xz(PlaneXZ);
            xz.SetPoint(center);
            Vec3f new_pos;
            GetCamera()->CastRay(cursorPos, xz, new_pos);

            center.z += new_pos.z - firstClick.z;

            firstClick = new_pos;
        }
    }

    void MoveablePoint::ClickEnd() {
        axisMoving = 0;
        moving = false;
    }

    void MoveablePoint::SetCursorPos(const Vec2f& _pos) {
        cursorPos = _pos;
        Calculate();
    }

    void MoveablePoint::Calculate() {
        Plane xz(PlaneXZ);
        xz.SetPoint(center);
        Plane xy(PlaneXY);
        xy.SetPoint(center);

        Vec3f r1;
        GetCamera()->CastRay(cursorPos, xz, r1);
        Vec3f r2;
        GetCamera()->CastRay(cursorPos, xy, r2);

        Vec2f dstz = r1;
        dstz -= center.AsVec2();
        Vec2f dstx(r1.z, r1.y);
        dstx -= Vec2f(center.z, center.y);
        Vec2f dsty(r2.z, r2.x);
        dsty -= Vec2f(center.z, center.x);

        if (moving == true) {
            return;
        }

        axisOver = 0;
        axisDrawK.Set(1, 1, 1);

        if (axisOZ == true && dstz.Length() < lineSizeWorld * 0.1 &&
            center.z < r1.z && r1.z < center.z + lineSizeWorld) {
            axisDrawK.z *= 0.5;
            axisOver = 3;
        } else if (axisOX == true && dstx.Length() < lineSizeWorld * 0.1 &&
                   center.x < r1.x && r1.x < center.x + lineSizeWorld) {
            axisDrawK.x *= 0.5;
            axisOver = 1;
        } else if (axisOY == true && dsty.Length() < lineSizeWorld * 0.1 &&
                   center.y < r2.y && r2.y < center.y + lineSizeWorld) {
            axisDrawK.y *= 0.5;
            axisOver = 2;
        }
    }

} // namespace drash
