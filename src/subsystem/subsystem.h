#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <cscene.h>

namespace drash
{

/// As our CScene class writen for objects creation, deletion end connection,
/// we need ability to extend CScene with different behaviors.
/// For example: explosion system, players system, network system, sound system
/// If objects creation is allowed, class instance must exist for whole life time
/// of scene objects, created from this subsystem
class CSubsystem
{
public:
    CSubsystem();
    ~CSubsystem();

    void SetScene(CScene *_scene);
    CScene *GetScene();

private:
    CScene *mScene;
};

}// namespace drash

#endif // SUBSYSTEM_H
