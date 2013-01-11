// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

#pragma once
#ifndef CSCENE_H
#define CSCENE_H

#include "../misc/vec2.h"
#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

class b2World;

namespace drash
{

class CPhysObserver;
class CSceneObjectGeometry;
class CSceneObjectParams;
class CSceneObject;
class CJoint;

class CSceneParams
{
public:
    CVec2f mGravity;
};

class CScene final
{
public:    
    // **************************************************
    // * static scene configuration *********************

    static const int mVelocityIterations = 5;
    static const int mPositionIterations = 2;
    static const unsigned int mObjectsCountLimit = 10000;
    static const unsigned int mSubsystemsCountLimit = 5;

    // **************************************************
    // * main routines **********************************

    CScene(void);
    ~CScene(void);

    bool Init( const CSceneParams& _params );
    void Release(void);

    /// must be called once in update cycle
    /// dt - seconds
    void Step( double _dt );

    // **************************************************
    // * working with objects ***************************

    CSceneObject* CreateObject(const CSceneObjectGeometry &_geometry, const CSceneObjectParams& _params );
    bool DestroyObject(CSceneObject *_obj);
    inline CSceneObject * const * GetObjects(void) const;
    inline unsigned int EnumObjects(void) const;
    void DestroyObjects(void);

    // **************************************************
    // * working with joints ****************************

    CJoint *CreateJoint(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor);
    void DestroyJoint(CJoint *_joint);

    void SetGravity(const CVec2f &_g);

    inline void SetPaused(bool _paused);

protected:
private:
    void DestroyObjectImpl(CSceneObject *_obj);

    bool mLocked = false;

    b2World *mWorld = nullptr;
    CPhysObserver *mObserver = nullptr;

    CObjectFactory<CSceneObject> mObjectsFactory;

    bool mPaused = false;
};

inline CSceneObject * const * CScene::GetObjects(void) const
{
    return mObjectsFactory.GetObjects();
}

inline unsigned int CScene::EnumObjects(void) const
{
    return mObjectsFactory.EnumObjects();
}

inline void CScene::SetPaused(bool _paused)
{
    mPaused = _paused;
}

} // namespace drash

#endif // CSCENE_H
