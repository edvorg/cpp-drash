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
    CCamera *CreateCam(const CCameraParams &_params);
    void DestroyCam(CCamera *_cam);
    void SetActiveCam(CCamera *_cam);
    CCamera *GetActiveCam();

    void Draw() const;

protected:
private:
    CCamera *mActiveCam = nullptr;
    std::vector<CCamera*> mCameras;
};

}// namespace drash

#endif // DEBUGDRAWSYSTEM_H
