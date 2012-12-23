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
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "camera.h"
#include "../misc/graphics.h"

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

bool CDebugDrawSystem::ScreenSpaceToWorldSpace(CVec2f &_pos, float _depth) const
{
    if (mActiveCam == nullptr)
    {
        return false;
    }
    else
    {
        if (mActiveCam->IsOrtho() == false)
        {
            // TODO: optimize this
            double fov = mActiveCam->GetFov().Get();

            double c = _depth / cos(fov / 2.0); // hypotenuse

            double frame_height = 2.0 * sqrt( c*c - _depth*_depth );
            double frame_width = frame_height * mAspectRatio;

            _pos.mX *= frame_width;
            _pos.mY *= frame_height;

            _pos += mActiveCam->GetPos().Get().Vec2();
        }
        else
        {
            _pos.mX *= GetActiveCam()->GetOrthoWidth().Get();
            _pos.mY *= GetActiveCam()->GetOrthoWidth().Get() / mAspectRatio;

            _pos += mActiveCam->GetPos().Get().Vec2();
        }

        return true;
    }
}

bool CDebugDrawSystem::WorldSpaceToScreenSpace(CVec2f &_pos, float _depth) const
{
    if (mActiveCam == nullptr)
    {
        return false;
    }
    else
    {
        if (mActiveCam->IsOrtho() == false)
        {
            // TODO: optimize this
            double fov = mActiveCam->GetFov().Get();

            double c = _depth / cos(fov / 2.0); // hypotenuse

            double frame_height = 2.0 * sqrt( c*c - _depth*_depth );
            double frame_width = frame_height * mAspectRatio;

            _pos -= mActiveCam->GetPos().Get().Vec2();

            _pos.mX /= frame_width;
            _pos.mY /= frame_height;
        }
        else
        {
            _pos -= mActiveCam->GetPos().Get().Vec2();

            _pos.mX /= GetActiveCam()->GetOrthoWidth().Get();
            _pos.mY /= GetActiveCam()->GetOrthoWidth().Get() / mAspectRatio;
        }

        return true;
    }
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

            float z = - cur_obj->GetPos().Get().mZ - cur_fgr->GetZ() + GetActiveCam()->GetPos().Get().mZ;
            CVec2f pos = _pos;
            ScreenSpaceToWorldSpace(pos, z);

            if (cur_fgr->mFixture->TestPoint(CVec2ToB2Vec2(pos)))
            {
                res = cur_fgr;
                z_nearest = z;
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

            float z = - cur_obj->GetPos().Get().mZ - cur_fgr->GetZ() + GetActiveCam()->GetPos().Get().mZ;

            if (z > z_nearest)
            {
                continue;
            }

            CVec2f pos = _pos;
            ScreenSpaceToWorldSpace(pos, z);

            if (cur_fgr->mFixture->TestPoint(CVec2ToB2Vec2(pos)))
            {
                res = cur_fgr;
                z_nearest = z;
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
    glLoadIdentity();

    if (mActiveCam->mOrtho == false)
    {
        gluPerspective(mActiveCam->GetFov().Get() * 180.0 / M_PI, mAspectRatio, 1.0f, mActiveCam->GetDepthOfView().Get());
    }
    else
    {
        float mhw = mActiveCam->GetOrthoWidth().Get() * 0.5f;
        float mhh = mhw / mAspectRatio;
        glOrtho(-mhw, mhw,
                -mhh, mhh,
                mActiveCam->GetPos().Get().mZ,
                mActiveCam->GetPos().Get().mZ - math::Abs(mActiveCam->GetDepthOfView().Get()));
    }

    for (unsigned int i=0; i<count; i++)
    {
        /////////////////////////////////////////////
        // frustrum culling
        /////////////////////////////////////////////

        CVec2f min(-0.5, -0.5);
        CVec2f max(0.5, 0.5);
        float d = - objects[i]->GetPos().Get().mZ + mActiveCam->GetPos().Get().mZ;

        ScreenSpaceToWorldSpace(min, d);
        ScreenSpaceToWorldSpace(max, d);

        objects[i]->ComputeBoundingBox();
        const b2AABB &aabb = objects[i]->GetBoundingBox();

        if (aabb.upperBound.x < min.mX ||
            max.mX < aabb.lowerBound.x ||
            aabb.upperBound.y < min.mY ||
            max.mY < aabb.lowerBound.y)
        {
            continue;
        }

        ////////////////////////////////////////////
        // drawing
        ////////////////////////////////////////////

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(mActiveCam->GetRotation().Get().mX, 1, 0, 0);
        glRotatef(mActiveCam->GetRotation().Get().mY, 0, 1, 0);
        glRotatef(mActiveCam->GetRotation().Get().mZ, 0, 0, 1);
        glTranslatef(-mActiveCam->mPos.Get().mX,
                     -mActiveCam->mPos.Get().mY,
                     -mActiveCam->mPos.Get().mZ);
        CVec2f pos = objects[i]->GetPos().Get().Vec2();
        glTranslatef(pos.mX,
                     pos.mY,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetAngle().Get(), 0, 0, 1 );

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        objects[i]->DrawDebug();
    }
}

void CDebugDrawSystem::DrawLine(const CVec2f _p1, const CVec2f _p2, const b2Color &_col) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBegin(GL_LINES);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p1.mX, _p1.mY);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p2.mX, _p2.mY);
    glEnd();
}

void CDebugDrawSystem::DrawLine(const CVec3f _p1, const CVec3f _p2, const b2Color &_col) const
{
    if (mActiveCam == nullptr)
    {
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(mActiveCam->GetFov().Get() * 180.0 / M_PI, mAspectRatio, 1.0f, mActiveCam->GetDepthOfView().Get());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBegin(GL_LINES);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p1.mX, _p1.mY);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p2.mX, _p2.mY);
    glEnd();
}

void CDebugDrawSystem::DrawPoint(const CVec2f _p, float _size, const b2Color &_col) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glPointSize(_size);

    glBegin(GL_POINTS);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p.mX, _p.mY);
    glEnd();
}

}// namespace drash
