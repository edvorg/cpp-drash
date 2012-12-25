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

#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

namespace drash
{

class CScene;

/// As our CScene class writen for objects creation, deletion end connection,
/// we need ability to extend CScene with different behaviors.
/// For example: explosion system, players system, network system, sound system
/// If objects creation is allowed, class instance must exist for whole life time
/// of scene objects, created from this subsystem
class CSubsystem
{
public:
    virtual bool Init() = 0;
    virtual void Step(double _dt) = 0;
    virtual void Release() = 0;

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
