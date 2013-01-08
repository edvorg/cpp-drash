// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "debugdrawsystem.h"

#if defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "../greng/camera.h"
#include "../misc/graphics.h"
#include "../misc/plane.h"
#include "../misc/ray.h"
#include "../misc/vec4.h"
#include "../misc/matrix4.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"
#include "../scene/scene.h"

namespace drash
{

bool CDebugDrawSystem::Init()
{
    return true;
}

void CDebugDrawSystem::Step(double _dt)
{
    for (auto i = mCameras.begin(); i != mCameras.end(); i++)
    {
        (*i)->Step(_dt);
    }

    if (mActiveCam != nullptr)
    {
        mViewMatrixTransposed = mActiveCam->GetViewMatrix();
        mProjectionMatrixTransposed = mActiveCam->GetProjectionMatrix();
        mViewMatrixTransposed.Transpose();
        mProjectionMatrixTransposed.Transpose();
    }
}

void CDebugDrawSystem::Release()
{
	mActiveCam = nullptr;

    for (auto i=mCameras.begin(); i!=mCameras.end(); i++)
    {
            delete *i;
    }

	mCameras.clear();
}

drash::CCamera *CDebugDrawSystem::CreateCam(const drash::CCameraParams &_params, bool _set_active)
{
    CCamera *res = new CCamera;

    if (res->Init(_params) == false)
    {
        delete res;
        return nullptr;
    }

    mCameras.push_back(res);

    res->SetAspectRatio(mAspectRatio);

    if (_set_active)
    {
        SetActiveCam(res);
    }

    return res;
}

void CDebugDrawSystem::DestroyCam(drash::CCamera *_cam)
{
    if (_cam == mActiveCam)
    {
        mActiveCam = nullptr;
    }

    for (auto i=mCameras.begin(); i!=mCameras.end(); i++)
    {
        if (*i == _cam)
        {
            delete *i;
            mCameras.erase(i);
            return;
        }
    }
}

void CDebugDrawSystem::SetActiveCam(CCamera *_cam)
{
    mActiveCam = _cam;

    if (mActiveCam != nullptr)
    {
        mViewMatrixTransposed = mActiveCam->GetViewMatrix();
        mProjectionMatrixTransposed = mActiveCam->GetProjectionMatrix();
        mViewMatrixTransposed.Transpose();
        mProjectionMatrixTransposed.Transpose();
    }
}

void CDebugDrawSystem::SetAspectRatio(float _ratio)
{
    if (drash::math::Abs(_ratio) < 0.000001)
    {
        _ratio = 1.0f;
    }

    mAspectRatio = _ratio;

    for (auto i = mCameras.begin(); i != mCameras.end(); i++)
    {
        (*i)->SetAspectRatio(_ratio);
    }
}

void CDebugDrawSystem::CastRay(const CVec2f &_pos, const CPlane &_plane, CVec3f &_result) const
{
    if (mActiveCam == nullptr)
    {
        return;
    }

    // TODO: optimize this
    double fov = mActiveCam->GetFov().Get();

    double c = 1.0 / cos(fov / 2.0); // hypotenuse

    double frame_height = 2.0 * sqrt(c*c - 1.0);
    double frame_width = frame_height * mAspectRatio;

    CVec2f pos = _pos;

    pos.mX *= frame_width;
    pos.mY *= frame_height;

    CVec4f dir(pos, -1, 1);
    CVec4f tmp;
    MatrixMultiply(mActiveCam->GetAntiRotationMatrix(), dir, tmp);

    CRay r;
    r.SetPoint(mActiveCam->GetPos().Get());
    r.SetDirection(tmp);
    _plane.CastRay(r, _result);
}

CFigure *CDebugDrawSystem::FindFigure(const CVec2f &_pos) const
{
    CFigure *res = nullptr;

    unsigned int i = 0;
    bool brk = false;
    float z_nearest = 0;

    for (i = 0; i < GetScene()->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = GetScene()->GetObjects()[i];

        for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++)
        {
            CFigure *cur_fgr = cur_obj->GetFigures()[j];

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

            CVec3f pos;
            CastRay(_pos, plane, pos);

            if (cur_fgr->TestPoint(pos))
            {
                res = cur_fgr;
                pos -= mActiveCam->GetPos().Get();
                z_nearest = pos.LengthSquared();
                brk = true;
            }

            if (brk == true)
            {
                break;
            }
        }

        if (brk == true)
        {
            break;
        }
    }

    for (; i < GetScene()->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = GetScene()->GetObjects()[i];

        for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++)
        {
            CFigure *cur_fgr = cur_obj->GetFigures()[j];

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

            CVec3f pos;
            CastRay(_pos, plane, pos);

            if (cur_fgr->TestPoint(pos))
            {
                pos -= mActiveCam->GetPos().Get();

                float z = pos.LengthSquared();

                if (z_nearest > z)
                {
                    res = cur_fgr;
                    z_nearest = z;
                }
            }
        }
    }

    return res;
}

void CDebugDrawSystem::Draw() const
{
    if (mActiveCam == nullptr)
    {
        LOG_WARN("CDebugDrawSystem::Draw(): no camera is activated");
        return;
    }

    CSceneObject * const * objects = GetScene()->GetObjects();
    unsigned int count = GetScene()->EnumObjects();

    glMatrixMode(GL_PROJECTION);

    if (mActiveCam->mOrtho == false)
    {
        glLoadMatrixf(mProjectionMatrixTransposed.mData);
    }
    else
    {
        glLoadIdentity();
        float mhw = mActiveCam->GetOrthoWidth().Get() * 0.5f;
        float mhh = mhw / mAspectRatio;
        glOrtho(-mhw, mhw,
                -mhh, mhh,
                mActiveCam->GetPos().Get().mZ,
                mActiveCam->GetPos().Get().mZ - math::Abs(mActiveCam->GetDepthOfView().Get()));
    }

    for (unsigned int i = 0; i < count; i++)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(mViewMatrixTransposed.mData);

        CVec2f pos = objects[i]->GetPos();
        glTranslatef(pos.mX,
                     pos.mY,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetAngle().Get(), 0, 0, 1 );

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        objects[i]->DrawDebug();
    }
}

void CDebugDrawSystem::DrawTriangle(const CVec2f &_p1, const CVec2f &_p2, const CVec2f &_p3, const CColor4f &_col, bool _depth_test) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLES);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p1.mX, _p1.mY);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p2.mX, _p2.mY);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p3.mX, _p3.mY);
    glEnd();
}

void CDebugDrawSystem::DrawTriangle(const CVec3f &_p1, const CVec3f &_p2, const CVec3f &_p3, const CColor4f &_col, bool _depth_test) const
{
    if (mActiveCam == nullptr)
    {
        return;
    }

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mViewMatrixTransposed.mData);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(mProjectionMatrixTransposed.mData);

    glBegin(GL_TRIANGLES);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p1.mX, _p1.mY, _p1.mZ);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p2.mX, _p2.mY, _p2.mZ);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p3.mX, _p3.mY, _p3.mZ);
    glEnd();
}

void CDebugDrawSystem::DrawLine(const CVec2f &_p1, const CVec2f &_p2, float _width, const CColor4f &_col, bool _depth_test) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(_width);

    glBegin(GL_LINES);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p1.mX, _p1.mY);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p2.mX, _p2.mY);
    glEnd();
}

void CDebugDrawSystem::DrawLine(const CVec3f &_p1, const CVec3f &_p2, float _width, const CColor4f &_col, bool _depth_test) const
{
    if (mActiveCam == nullptr)
    {
        return;
    }

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mViewMatrixTransposed.mData);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(mProjectionMatrixTransposed.mData);

    glLineWidth(_width);

    glBegin(GL_LINES);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p1.mX, _p1.mY, _p1.mZ);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p2.mX, _p2.mY, _p2.mZ);
    glEnd();
}

void CDebugDrawSystem::DrawPoint(const CVec2f &_p, float _size, const CColor4f &_col, bool _depth_test) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(_size);

    glBegin(GL_POINTS);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex2f(_p.mX, _p.mY);
    glEnd();
}

void CDebugDrawSystem::DrawPoint(const CVec3f &_p, float _size, const CColor4f &_col, bool _depth_test) const
{
    if (mActiveCam == nullptr)
    {
        return;
    }

    if (_depth_test == true)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mViewMatrixTransposed.mData);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(mProjectionMatrixTransposed.mData);

    glPointSize(_size);

    glBegin(GL_POINTS);
    glColor4f(_col.mR, _col.mG, _col.mB, _col.mA);
    glVertex3f(_p.mX, _p.mY, _p.mZ);
    glEnd();
}

}// namespace drash
