#include "rotationablepoint.h"

#include "../misc/vec4.h"
#include "../misc/matrix4.h"
#include "../misc/plane.h"

#include "../greng/renderer.h"
#include "../greng/camera.h"

namespace drash
{

bool CRotationablePoint::Init()
{
    if (mRenderer == nullptr ||
        mCamera == nullptr)
    {
        return false;
    }

    Release();

    return true;
}

void CRotationablePoint::Step(double)
{
    CMatrix4f rotx;
    MatrixRotationX(rotx, mRotation.mX);

    CMatrix4f rotz;
    MatrixRotationZ(rotz, mRotation.mZ);

    CMatrix4f roty;
    MatrixRotationY(roty, mRotation.mY);

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
    xy.SetPoint(mPoint);
    xy.SetNormal(xy_normal_transposed);

    CPlane xz;
    xz.SetPoint(mPoint);
    xz.SetNormal(xz_normal_transposed);

    CPlane yz;
    yz.SetPoint(mPoint);
    yz.SetNormal(yz_normal_transposed);

    CVec3f xy_proj;
    mCamera->CastRay(mCursorPos, xy, xy_proj);

    CVec3f xz_proj;
    mCamera->CastRay(mCursorPos, xz, xz_proj);

    CVec3f yz_proj;
    mCamera->CastRay(mCursorPos, yz, yz_proj);

    xy_proj -= mPoint;
    xz_proj -= mPoint;
    yz_proj -= mPoint;

    if (mAxisRotating == 0)
    {
        float len_xy = xy_proj.Length();
        float len_xz = xz_proj.Length();
        float len_yz = yz_proj.Length();

        if (radius - 0.1 < len_xy && len_xy < radius + 0.1)
        {
            mAxisOvered = 3;
            return;
        }
        else if (radius - 0.1 < len_xz && len_xz < radius + 0.1)
        {
            mAxisOvered = 2;
            return;
        }
        else if (radius - 0.1 < len_yz && len_yz < radius + 0.1)
        {
            mAxisOvered = 1;
            return;
        }

        mAxisOvered = 0;
    }
    else
    {
        if (mAxisRotating == 3)
        {
            xy_proj.Normalize();

            mRotation.mZ = acos(xy_proj.mX);
            mRotation.mZ = (xy_proj.mY > 0.000001f ? mRotation.mZ : -mRotation.mZ);
        }
        else if (mAxisRotating == 2)
        {
            xz_proj.Normalize();

            mRotation.mY = acos(xz_proj.mZ);
            mRotation.mY = (xz_proj.mX > 0.000001f ? mRotation.mY : -mRotation.mY);
        }
        else if (mAxisRotating == 1)
        {
            yz_proj.Normalize();

            mRotation.mX = acos(yz_proj.mZ);
            mRotation.mX = (yz_proj.mY < -0.000001f ? mRotation.mX : -mRotation.mX);
        }
    }
}

void CRotationablePoint::Render()
{
    unsigned int segments = 32;

    float angle = 0;
    float angle_delta = 2.0 * M_PI / segments;

    CMatrix4f rotx;
    MatrixRotationX(rotx, mRotation.mX);

    CMatrix4f rotz;
    MatrixRotationZ(rotz, mRotation.mZ);

    CMatrix4f roty;
    MatrixRotationY(roty, mRotation.mY);

    CMatrix4f rot_yz;
    MatrixMultiply(roty, rotz, rot_yz);

    CMatrix4f rot;
    MatrixMultiply(rotx, rot_yz, rot);

    for (unsigned int i = 0; i < segments; i++)
    {
        CVec4f p1(radius * cos(angle), radius * sin(angle), 0, 0);
        CVec4f p2(radius * cos(angle + angle_delta), radius * sin(angle + angle_delta), 0, 0);

        CVec4f p1_transposed;
        CVec4f p2_transposed;

        MatrixMultiply(rot, p1, p1_transposed);
        MatrixMultiply(rot, p2, p2_transposed);

        mRenderer->DrawLine(mCamera, p1_transposed, p2_transposed, 1, CColor4f(0, 0, 1, mAxisOvered == 3 ? 0.5 : 1), false);

        angle += angle_delta;
    }

    CVec4f x(1, 0, 0, 0);

    CVec4f x_transposed;
    MatrixMultiply(rot, x, x_transposed);

    x_transposed.Vec3().Normalize();
    x_transposed.Vec3() *= radius;
    x_transposed.Vec3() += mPoint;

    mRenderer->DrawLine(mCamera, mPoint, x_transposed, 1, CColor4f(1, 0, 0, 1), false);

    ///

    for (unsigned int i = 0; i < segments; i++)
    {
        CVec4f p1(radius * cos(angle), 0, - radius * sin(angle), 0);
        CVec4f p2(radius * cos(angle + angle_delta), 0, - radius * sin(angle + angle_delta), 0);

        CVec4f p1_transposed;
        CVec4f p2_transposed;

        MatrixMultiply(rot, p1, p1_transposed);
        MatrixMultiply(rot, p2, p2_transposed);

        mRenderer->DrawLine(mCamera, p1_transposed, p2_transposed, 1, CColor4f(0, 1, 0, mAxisOvered == 2 ? 0.5 : 1), false);

        angle += angle_delta;
    }

    CVec4f z(0, 0, 1, 0);

    CVec4f z_transposed;
    MatrixMultiply(rot, z, z_transposed);

    z_transposed.Vec3().Normalize();
    z_transposed.Vec3() *= radius;
    z_transposed.Vec3() += mPoint;

    mRenderer->DrawLine(mCamera, mPoint, z_transposed, 1, CColor4f(0, 0, 1, 1), false);

    ///

    for (unsigned int i = 0; i < segments; i++)
    {
        CVec4f p1(0, radius * sin(angle), - radius * cos(angle), 0);
        CVec4f p2(0, radius * sin(angle + angle_delta), - radius * cos(angle + angle_delta), 0);

        CVec4f p1_transposed;
        CVec4f p2_transposed;

        MatrixMultiply(rot, p1, p1_transposed);
        MatrixMultiply(rot, p2, p2_transposed);

        mRenderer->DrawLine(mCamera, p1_transposed, p2_transposed, 1, CColor4f(1, 0, 0, mAxisOvered == 1 ? 0.5 : 1), false);

        angle += angle_delta;
    }

    CVec4f y(0, 1, 0, 0);

    CVec4f y_transposed;
    MatrixMultiply(rot, y, y_transposed);

    y_transposed.Vec3().Normalize();
    y_transposed.Vec3() *= radius;
    y_transposed.Vec3() += mPoint;

    mRenderer->DrawLine(mCamera, mPoint, y_transposed, 1, CColor4f(0, 1, 0, 1), false);
}

void CRotationablePoint::Release()
{
}

void CRotationablePoint::RotateBegin()
{
    mAxisRotating = mAxisOvered;
}

void CRotationablePoint::RotateEnd()
{
    mAxisRotating = 0;
}

} // namespace drash
