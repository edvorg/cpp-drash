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

#ifndef DEBUGDRAWSYSTEM_H
#define DEBUGDRAWSYSTEM_H

#include "subsystem.h"
#include "../sceneobjects.h"
#include <vector>

namespace drash
{

class CDebugDrawSystem : public CSubsystem
{
public:
    CCamera *CreateCam(const CCameraParams &_params, bool _set_active = false);
    void DestroyCam(CCamera *_cam);

    inline void SetActiveCam(CCamera *_cam);
    inline CCamera *GetActiveCam();

    inline void SetAspectRatio(float _ratio);
    inline float GetAspectRatio() const;

    /// converts coordinates from (-0.5,-0.5)..(0.5, 0.5) system with center at (0, 0)
    /// to world coordinates taking into account depth (distance from camera to required layer)
    /// and active camera position
    /// if no camera is activated does nothing and returns false
    bool ScreenSpaceToWorldSpace(CVec2 &_pos, float _depth) const;

    /// finds objects, visible at specified postion in screen space coordinates
    /// returns nearest one
    CSceneObject *FindObject(const CVec2 &_pos);

    void Draw() const;

protected:
private:
    CCamera *mActiveCam = nullptr;
    std::vector<CCamera*> mCameras;
    float mAspectRatio = 1;
};

inline void CDebugDrawSystem::SetActiveCam(CCamera *_cam)
{
    mActiveCam = _cam;
}

inline CCamera *CDebugDrawSystem::GetActiveCam()
{
    return mActiveCam;
}

inline void CDebugDrawSystem::SetAspectRatio(float _ratio)
{
    mAspectRatio = _ratio;
}

inline float CDebugDrawSystem::GetAspectRatio() const
{
    return mAspectRatio;
}

}// namespace drash

#endif // DEBUGDRAWSYSTEM_H
