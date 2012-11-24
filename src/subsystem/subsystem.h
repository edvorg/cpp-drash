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
    ~CSubsystem();

    void SetScene(CScene *_scene);
    inline CScene *GetScene();
    inline const CScene *GetScene() const;

private:
    CScene *mScene = nullptr;
};

inline CScene *CSubsystem::GetScene()
{
    return mScene;
}

inline const CScene *CSubsystem::GetScene() const
{
    return mScene;
}

}// namespace drash

#endif // SUBSYSTEM_H
