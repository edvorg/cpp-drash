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

#include "../misc/graphics.h"

namespace drash
{

drash::CCamera *CDebugDrawSystem::CreateCam(const drash::CCameraParams &_params, bool _set_active)
{
    if (GetScene() == nullptr)
    {
        return nullptr;
    }

    drash::CSceneObjectGeometry g;
    mCameras.push_back(GetScene()->CreateObject<CCamera>(g, _params));
    if (_set_active)
    {
        SetActiveCam(mCameras.back());
    }
    return mCameras.back();
}

void CDebugDrawSystem::DestroyCam(drash::CCamera *_cam)
{
    for (auto i=mCameras.begin(); i!=mCameras.end(); i++)
    {
        if (*i == _cam)
        {
            if (GetScene() != nullptr)
            {
                GetScene()->DestroyObject(_cam);
            }

            mCameras.erase(i);
            return;
        }
    }
}

bool CDebugDrawSystem::ScreenSpaceToWorldSpace(CVec2 &_pos, float _depth) const
{
    if (mActiveCam == nullptr)
    {
        return false;
    }
    else
    {
//        TODO: optimize this
        double fov = mActiveCam->GetFov();

        double c = _depth / cos(fov / 2.0); // hypotenuse

        double frame_height = 2.0 * sqrt( c*c - _depth*_depth );
        double frame_width = frame_height * mAspectRatio;

        _pos.x *= frame_width;
        _pos.y *= frame_height;

        _pos += mActiveCam->GetPos().Get().Vec2();

        return true;
    }
}

bool CDebugDrawSystem::WorldSpaceToScreenSpace(CVec2 &_pos, float _depth) const
{
    if (mActiveCam == nullptr)
    {
        return false;
    }
    else
    {
//        TODO: optimize this
        double fov = mActiveCam->GetFov();

        double c = _depth / cos(fov / 2.0); // hypotenuse

        double frame_height = 2.0 * sqrt( c*c - _depth*_depth );
        double frame_width = frame_height * mAspectRatio;

        _pos -= mActiveCam->GetPos().Get().Vec2();

        _pos.x /= frame_width;
        _pos.y /= frame_height;

        return true;
    }
}

CFigure *CDebugDrawSystem::FindFigure(const CVec2 &_pos) const
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
            CVec2 pos = _pos;
            ScreenSpaceToWorldSpace(pos, z);

            if (cur_fgr->mFixture->TestPoint(pos))
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

            CVec2 pos = _pos;
            ScreenSpaceToWorldSpace(pos, z);

            if (cur_fgr->mFixture->TestPoint(pos))
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
    gluPerspective(mActiveCam->GetFov() * 180.0 / M_PI, mAspectRatio, 1.0f, mActiveCam->GetDepthOfView());

    for (unsigned int i=0; i<count; i++)
    {
        /////////////////////////////////////////////
        // frustrum culling
        /////////////////////////////////////////////

        CVec2 min(-0.5, -0.5);
        CVec2 max(0.5, 0.5);
        float d = - objects[i]->GetPos().Get().mZ + mActiveCam->GetPos().Get().mZ;

        ScreenSpaceToWorldSpace(min, d);
        ScreenSpaceToWorldSpace(max, d);

        objects[i]->ComputeBoundingBox();
        const b2AABB &aabb = objects[i]->GetBoundingBox();

        if (aabb.upperBound.x < min.x ||
            max.x < aabb.lowerBound.x ||
            aabb.upperBound.y < min.y ||
            max.y < aabb.lowerBound.y)
        {
            continue;
        }

        ////////////////////////////////////////////
        // drawing
        ////////////////////////////////////////////

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-mActiveCam->mPos.Get().mX,
                     -mActiveCam->mPos.Get().mY,
                     -mActiveCam->mPos.Get().mZ);
        CVec2 pos = objects[i]->GetPos().Get().Vec2();
        glTranslatef(pos.x,
                     pos.y,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetAngle().Get(), 0, 0, 1 );

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        objects[i]->DrawDebug();
    }
}

void CDebugDrawSystem::DrawLine(const CVec2 _p1, const CVec2 _p2, const b2Color &_col) const
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
    glVertex2f(_p1.x, _p1.y);
    glColor3f(_col.r, _col.g, _col.b);
    glVertex2f(_p2.x, _p2.y);
    glEnd();
}

void CDebugDrawSystem::DrawPoint(const CVec2 _p, float _size, const b2Color &_col) const
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
    glVertex2f(_p.x, _p.y);
    glEnd();
}

}// namespace drash
