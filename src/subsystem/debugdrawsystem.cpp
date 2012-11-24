#include "debugdrawsystem.h"

#include <GL/gl.h>

namespace drash
{

drash::CCamera *CDebugDrawSystem::CreateCam(const drash::CCameraParams &_params)
{
    if (GetScene() == nullptr)
    {
        return nullptr;
    }

    drash::CSceneObjectGeometry g;
    mCameras.push_back(GetScene()->CreateObject<CCamera>(g, _params));
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

void CDebugDrawSystem::SetActiveCam(CCamera *_cam)
{
    mActiveCam = _cam;
}

CCamera *CDebugDrawSystem::GetActiveCam()
{
    return mActiveCam;
}

void CDebugDrawSystem::SetAspectRatio(float _ratio)
{
    mAspectRatio = _ratio;
}

float CDebugDrawSystem::GetAspectRatio() const
{
    return mAspectRatio;
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
        double fov = 60.0 * M_PI / 180.0;

        double c = _depth / cos(fov / 2.0); // hypotenuse

        double frame_height = 2.0 * sqrt( c*c - _depth*_depth );
        double frame_width = frame_height * mAspectRatio;

        _pos.x *= frame_width;
        _pos.y *= frame_height;

        _pos += mActiveCam->GetPos().Get();

        return true;
    }
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

    for (unsigned int i=0; i<count; i++)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-mActiveCam->mPos.Get().x,
                     -mActiveCam->mPos.Get().y,
                     -mActiveCam->GetZ().Get());
        glTranslatef(objects[i]->GetBody()->GetWorldCenter().x,
                     objects[i]->GetBody()->GetWorldCenter().y,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetBody()->GetAngle(), 0, 0, 1 );

        objects[i]->DrawDebug();
    }
}

}// namespace drash