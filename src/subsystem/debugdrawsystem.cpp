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

#include <GL/gl.h>
#include <GL/glu.h>
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

        _pos += mActiveCam->GetPos().Get();

        return true;
    }
}

CSceneObject *CDebugDrawSystem::FindObject(const CVec2 &_pos)
{
    const drash::CScene::ObjectsT &objects = GetScene()->GetObjects();
    unsigned int count = GetScene()->EnumObjects();

    CSceneObject *res = nullptr;

    for (unsigned int i=0; i<count; i++)
    {
        CVec2 world_pos = _pos;
        ScreenSpaceToWorldSpace(world_pos, objects[i]->GetZ().Get() - mActiveCam->GetZ().Get());

        if (objects[i]->TestPoint(world_pos, objects[i]->GetZ().Get()) == true)
        {
            res = objects[i];
            break;
        }
    }

    if (res != nullptr)
    {
        for (unsigned int i=0; i<count; i++)
        {
            if (objects[i]->GetZ().Get() < res->GetZ().Get())
            {
                continue;
            }

            CVec2 world_pos = _pos;
            ScreenSpaceToWorldSpace(world_pos, objects[i]->GetZ().Get() - mActiveCam->GetZ().Get());

            if (objects[i]->TestPoint(world_pos, objects[i]->GetZ().Get()) == true)
            {
                res = objects[i];
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

    const drash::CScene::ObjectsT &objects = GetScene()->GetObjects();
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
        float d = drash::math::Abs(objects[i]->GetZ().Get() - mActiveCam->GetZ().Get());

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
        glTranslatef(-mActiveCam->mPos.Get().x,
                     -mActiveCam->mPos.Get().y,
                     -mActiveCam->GetZ().Get());
        glTranslatef(objects[i]->GetBody()->GetWorldCenter().x,
                     objects[i]->GetBody()->GetWorldCenter().y,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetBody()->GetAngle(), 0, 0, 1 );

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        objects[i]->DrawDebug();
    }
}

}// namespace drash
