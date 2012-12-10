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

#pragma once
#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "figure.h"
#include "../misc/animatedparam.h"

namespace drash
{

class CScene;
class CSceneObject;
class CExplosionParams;

class CSceneObjectGeometry
{
public:
    std::vector<CFigureParams> mFigures;

protected:
private:
};

class CSceneObjectParams
{
public:
    bool mDynamic = true;
    CVec2 mPos = CVec2(0);
    float mAngle = 0;
    bool mFixedRotation = false;
    float mZ = 0;
};

class CContact
{
public:
    CVec2 mPoints[b2_maxManifoldPoints];
    unsigned int mPointCount;
    CSceneObject *obj;
};

class CSceneObject
{
public:
    friend class CPhysObserver;
    friend class CScene;
    friend class CExplosion;
    friend class CDebugDrawSystem;

    static const unsigned int mFiguresCountLimit = 16;

    typedef CSceneObjectGeometry GeometryT;
    typedef CSceneObjectParams ParamsT;

    inline CScene *GetScene();
    inline const CScene *GetScene() const;

    CFigure *CreateFigure(const CFigureParams &_params);
    void DestroyFigure(CFigure *_figure);
    inline CFigure * const *GetFigures() const;
    inline unsigned int EnumFigures() const;

    inline void SetDynamic(bool _dynamic);
    inline bool IsDynamic() const;
    inline void ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos );
    inline void SetLinearVelocity(const CVec2 &_vel);
    inline CVec2 GetLinearVelocity() const;
    inline void SetAngularVelocity(float _vel);
    inline float GetAngularVelocity() const;
    inline void SetFixedRotation( bool _fixed );
    inline void SetActive( bool _active );
    inline CVec2 GetWorldPoint(const CVec2 &_local_point) const;

    inline CAnimatedParam<CVec2> &GetPos();
    inline CAnimatedParam<float> &GetAngle();
    inline CAnimatedParam<float> &GetZ();

    virtual void ComputeBoundingBox();
    inline const b2AABB &GetBoundingBox() const;

    friend CLogger &operator <<(CLogger &_logger, const CSceneObject &_object);

protected:
    CSceneObject(void);
    virtual ~CSceneObject(void);

    bool Init( const GeometryT &_geometry, const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnContactPreSolve( const CContact &_contact );
    virtual void OnContactEnd( const CContact &_contact );
    virtual void OnBoom( const CExplosionParams &_boom );
    virtual void DrawDebug() const;

    b2AABB mBoundingBox;

private:
    b2Body* mBody = nullptr;
    CScene* mScene = nullptr;
    bool mDead = false;
    int mInternalId = -1;
    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;
    float mColor[3];
    CFigure *mFigures[mFiguresCountLimit];
    unsigned int mFiguresCount = 0;
    CAnimatedParam<float> mZ;
};

inline CScene *CSceneObject::GetScene()
{
    return mScene;
}

inline const CScene *CSceneObject::GetScene() const
{
    return mScene;
}

inline void CSceneObject::SetDynamic( bool _dynamic )
{
    mBody->SetType( _dynamic ? b2_dynamicBody : b2_kinematicBody );
}

inline bool CSceneObject::IsDynamic() const
{
    return mBody->GetType() == b2_dynamicBody ? true : false;
}

inline CFigure * const *CSceneObject::GetFigures() const
{
    return mFigures;
}

inline unsigned int CSceneObject::EnumFigures() const
{
    return mFiguresCount;
}

inline void CSceneObject::ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos )
{
    mBody->ApplyLinearImpulse( _dir, _pos );
}

inline void CSceneObject::SetLinearVelocity(const CVec2 &_vel)
{
    mBody->SetLinearVelocity(_vel);
}

inline CVec2 CSceneObject::GetLinearVelocity() const
{
    return mBody->GetLinearVelocity();
}

inline void CSceneObject::SetAngularVelocity(float _vel)
{
    mBody->SetAngularVelocity(_vel);
}

float CSceneObject::GetAngularVelocity() const
{
    return mBody->GetAngularVelocity();
}

inline void CSceneObject::SetFixedRotation( bool _fixed )
{
    mBody->SetFixedRotation(_fixed);
}

inline void CSceneObject::SetActive( bool _active )
{
    mBody->SetActive(_active);
}

CVec2 CSceneObject::GetWorldPoint(const CVec2 &_local_point) const
{
    return mBody->GetWorldPoint(_local_point);
}

inline CAnimatedParam<CVec2> &CSceneObject::GetPos()
{
    return mPos;
}

inline CAnimatedParam<float> &CSceneObject::GetAngle()
{
    return mAngle;
}

inline CAnimatedParam<float> &CSceneObject::GetZ()
{
    return mZ;
}

inline const b2AABB &CSceneObject::GetBoundingBox() const
{
    return mBoundingBox;
}

} // namespace drash

#endif // CSCENEOBJECT_H
