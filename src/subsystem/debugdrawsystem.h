#ifndef DEBUGDRAWSYSTEM_H
#define DEBUGDRAWSYSTEM_H

#include "subsystem.h"
#include "../sceneobjects.h"

namespace drash
{

class CDebugDrawSystem : public CSubsystem
{
public:
    void Draw(const CCamera &_camera) const;
};

}// namespace drash

#endif // DEBUGDRAWSYSTEM_H
