#include "moveablepoint.h"
#include "plane.h"

namespace drash{


CMoveablePoint::CMoveablePoint()
{
    mCenter.Set(0,0,0);
}

CMoveablePoint::CMoveablePoint(CVec3f _point, greng::CCamera *_camera):
    mCenter(_point), mCurrentCamera(_camera)
{
}


void CMoveablePoint::Colculate()
{
    mX = mCenter;
    mY = mCenter;
    mZ = mCenter;
    mX.mX += 10;
    mY.mY += 10;
    mZ.mZ += 10;

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

    if (dstz.Length() < 3)
    {
        mAxisDrawK.mZ *= 0.5;
        mAxisOver = 3;
    }
    else if (dstx.Length() < 3)
    {
        mAxisDrawK.mX *= 0.5;
        mAxisOver = 1;
    }
    else if (dsty.Length() < 3)
    {
        mAxisDrawK.mY *= 0.5;
        mAxisOver = 2;
    }
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

void CMoveablePoint::Render(greng::CRenderer &_render )
{
    if (mCurrentCamera == nullptr) {
        return;
    }
   _render.DrawLine(GetCamera(), mCenter, mX, 1, CColor4f(1 * mAxisDrawK.mX, 0, 0, 1), false);
   _render.DrawLine(GetCamera(), mCenter, mY, 1, CColor4f(0, 1 * mAxisDrawK.mY, 0, 1), false);
   _render.DrawLine(GetCamera(), mCenter, mZ, 1, CColor4f(0, 0, 1 * mAxisDrawK.mZ, 1), false);
}

void CMoveablePoint::SetCursorPos(const CVec2f &_pos)
{
    mCursorPos = _pos;
    Colculate();
}

} // namespace drash
