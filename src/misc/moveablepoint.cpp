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

    CMoveablePoint::CMoveablePoint() { center.Set(0, 0, 0); }

    CMoveablePoint::CMoveablePoint(CVec3f _point, greng::CCamera* _camera)
        : currentCamera(_camera), center(_point) {}

    void CMoveablePoint::SetCenter(const CVec3f& _center) { center = _center; }

    void CMoveablePoint::SetCamera(greng::CCamera* _camera) {
        currentCamera = _camera;
    }

    void CMoveablePoint::Step(double) {
        if (currentCamera == nullptr) {
            return;
        }

        CVec4f normal(0, 0, -1, 0);
        CVec4f normal_transformed;
        MatrixMultiply(currentCamera->GetAntiRotationMatrix(), normal,
                       normal_transformed);

        CPlane p;
        p.SetPoint(center);
        p.SetNormal(normal_transformed);

        CVec2f p1(0, 0);
        CVec2f p2(lineSizeScreen, 0);

        CVec3f wp1;
        CVec3f wp2;

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

    void CMoveablePoint::Render(greng::CRenderer& _render) {
        if (currentCamera == nullptr) {
            return;
        }
        _render.DrawLine(*GetCamera(), center, x, 1,
                         CColor4f(1 * axisDrawK.x + 10, 0, 0, 1), false);
        _render.DrawLine(*GetCamera(), center, y, 1,
                         CColor4f(0, 1 * axisDrawK.y + 10, 0, 1), false);
        _render.DrawLine(*GetCamera(), center, z, 1,
                         CColor4f(0, 0, 1 * axisDrawK.z + 10, 1), false);
    }

    greng::CCamera* CMoveablePoint::GetCamera() { return currentCamera; }

    void CMoveablePoint::ClickBegin() {
        if (axisOver == 0) {
            return;
        }
        if (axisOver == 2) {
            CPlane xy(PlaneXY);
            xy.SetPoint(center);
            GetCamera()->CastRay(cursorPos, xy, firstClick);
            axisMoving = axisOver;
            moving = true;
        } else {
            CPlane xz(PlaneXZ);
            xz.SetPoint(center);
            GetCamera()->CastRay(cursorPos, xz, firstClick);
            axisMoving = axisOver;
            moving = true;
        }
    }

    void CMoveablePoint::ClickPressing() {
        if (axisMoving == 1) {
            CPlane xz(PlaneXZ);
            xz.SetPoint(center);
            CVec3f new_pos;
            GetCamera()->CastRay(cursorPos, xz, new_pos);

            center.x += new_pos.x - firstClick.x;
            firstClick = new_pos;
        } else if (axisMoving == 2) {
            CPlane xy(PlaneXY);
            xy.SetPoint(center);
            CVec3f new_pos;
            GetCamera()->CastRay(cursorPos, xy, new_pos);

            center.y += new_pos.y - firstClick.y;
            firstClick = new_pos;
        } else if (axisMoving == 3) {
            CPlane xz(PlaneXZ);
            xz.SetPoint(center);
            CVec3f new_pos;
            GetCamera()->CastRay(cursorPos, xz, new_pos);

            center.z += new_pos.z - firstClick.z;

            firstClick = new_pos;
        }
    }

    void CMoveablePoint::ClickEnd() {
        axisMoving = 0;
        moving = false;
    }

    void CMoveablePoint::SetCursorPos(const CVec2f& _pos) {
        cursorPos = _pos;
        Calculate();
    }

    void CMoveablePoint::Calculate() {
        CPlane xz(PlaneXZ);
        xz.SetPoint(center);
        CPlane xy(PlaneXY);
        xy.SetPoint(center);

        CVec3f r1;
        GetCamera()->CastRay(cursorPos, xz, r1);
        CVec3f r2;
        GetCamera()->CastRay(cursorPos, xy, r2);

        CVec2f dstz = r1;
        dstz -= center.Vec2();
        CVec2f dstx(r1.z, r1.y);
        dstx -= CVec2f(center.z, center.y);
        CVec2f dsty(r2.z, r2.x);
        dsty -= CVec2f(center.z, center.x);

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
