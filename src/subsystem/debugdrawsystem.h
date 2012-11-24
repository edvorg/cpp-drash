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
