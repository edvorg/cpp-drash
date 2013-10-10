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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "moveablepoint.h"
#include "plane.h"

namespace drash
{

CMoveablePoint::CMoveablePoint()
{
    mCenter.Set(0,0,0);
}

CMoveablePoint::CMoveablePoint(CVec3f _point, greng::CCamera *_camera):
    mCurrentCamera(_camera),
    mCenter(_point)
{
}

void CMoveablePoint::SetCenter(const CVec3f &_center)
{
    mCenter = _center;

}

void CMoveablePoint::SetCamera(greng::CCamera *_camera)
{
    mCurrentCamera = _camera;
}

void CMoveablePoint::Step(double)
{
    if (mCurrentCamera == nullptr)
    {
        return;
    }

    CVec4f normal(0, 0, -1, 0);
    CVec4f normal_transformed;
    MatrixMultiply(mCurrentCamera->GetAntiRotationMatrix(), normal, normal_transformed);

    CPlane p;
    p.SetPoint(mCenter);
    p.SetNormal(normal_transformed);

    CVec2f p1(0, 0);
    CVec2f p2(mLineSizeScreen, 0);

    CVec3f wp1;
    CVec3f wp2;

    mCurrentCamera->CastRay(p1, p, wp1);
    mCurrentCamera->CastRay(p2, p, wp2);

    wp1 -= wp2;

    mLineSizeWorld = wp1.Length();

    mX = mCenter;
    mY = mCenter;
    mZ = mCenter;

    mX.mX += mLineSizeWorld;
    mY.mY += mLineSizeWorld;
    mZ.mZ += mLineSizeWorld;
}

void CMoveablePoint::Render( greng::CRenderer &_render )
{
    if (mCurrentCamera == nullptr) {
        return;
    }
   _render.DrawLine(GetCamera(), mCenter, mX, 1, CColor4f(1 * mAxisDrawK.mX+10, 0, 0, 1), false);
   _render.DrawLine(GetCamera(), mCenter, mY, 1, CColor4f(0, 1 * mAxisDrawK.mY+10, 0, 1), false);
   _render.DrawLine(GetCamera(), mCenter, mZ, 1, CColor4f(0, 0, 1 * mAxisDrawK.mZ+10, 1), false);
}

greng::CCamera *CMoveablePoint::GetCamera()
{
    return mCurrentCamera;
}

void CMoveablePoint::ClickBegin()
{
    if (mAxisOver == 0) {
        return;
    }
    if (mAxisOver == 2) {
            CPlane xy(PlaneXY);
            xy.SetPoint(mCenter);
            GetCamera()->CastRay(mCursorPos, xy, mFirstClick);
            mAxisMoving = mAxisOver;
            mMoving = true;
    } else {
            CPlane xz(PlaneXZ);
            xz.SetPoint(mCenter);
            GetCamera()->CastRay(mCursorPos, xz, mFirstClick);
            mAxisMoving = mAxisOver;
            mMoving = true;
    }
}


void CMoveablePoint::ClickPressing()
{
    if (mAxisMoving == 1)
    {
        CPlane xz(PlaneXZ);
        xz.SetPoint(mCenter);
        CVec3f new_pos;
        GetCamera()->CastRay(mCursorPos, xz, new_pos);

        mCenter.mX += new_pos.mX - mFirstClick.mX;
        mFirstClick = new_pos;
    }
    else if (mAxisMoving == 2)
    {
        CPlane xy(PlaneXY);
        xy.SetPoint(mCenter);
        CVec3f new_pos;
        GetCamera()->CastRay(mCursorPos, xy, new_pos);

        mCenter.mY += new_pos.mY - mFirstClick.mY;
        mFirstClick = new_pos;
    }
    else if (mAxisMoving == 3)
    {
        CPlane xz(PlaneXZ);
        xz.SetPoint(mCenter);
        CVec3f new_pos;
        GetCamera()->CastRay(mCursorPos, xz, new_pos);

        mCenter.mZ += new_pos.mZ - mFirstClick.mZ;

        mFirstClick = new_pos;
    }
}

void CMoveablePoint::ClickEnd()
{
    mAxisMoving = 0;
    mMoving = false;
}

void CMoveablePoint::SetCursorPos(const CVec2f &_pos)
{
    mCursorPos = _pos;
    Calculate();
}

void CMoveablePoint::Calculate()
{
    CPlane xz(PlaneXZ);
    xz.SetPoint(mCenter);
    CPlane xy(PlaneXY);
    xy.SetPoint(mCenter);

    CVec3f r1;
    GetCamera()->CastRay(mCursorPos, xz, r1);
    CVec3f r2;
    GetCamera()->CastRay(mCursorPos, xy, r2);

    CVec2f dstz = r1;
    dstz -= mCenter.Vec2();
    CVec2f dstx(r1.mZ, r1.mY);
    dstx -= CVec2f(mCenter.mZ, mCenter.mY);
    CVec2f dsty(r2.mZ, r2.mX);
    dsty -= CVec2f(mCenter.mZ, mCenter.mX);

    if (mMoving == true) {
        return;
    }

    mAxisOver = 0;
    mAxisDrawK.Set(1, 1, 1);

    if (mAxisOZ == true && dstz.Length() < mLineSizeWorld * 0.1 &&
        mCenter.mZ < r1.mZ &&
        r1.mZ < mCenter.mZ + mLineSizeWorld)
    {
        mAxisDrawK.mZ *= 0.5;
        mAxisOver = 3;
    }
    else if (mAxisOX == true && dstx.Length() < mLineSizeWorld * 0.1 &&
             mCenter.mX < r1.mX &&
             r1.mX < mCenter.mX + mLineSizeWorld)
    {
        mAxisDrawK.mX *= 0.5;
        mAxisOver = 1;
    }
    else if (mAxisOY == true && dsty.Length() < mLineSizeWorld * 0.1 &&
             mCenter.mY < r2.mY &&
             r2.mY < mCenter.mY + mLineSizeWorld)
    {
        mAxisDrawK.mY *= 0.5;
        mAxisOver = 2;
    }
}

} // namespace drash
