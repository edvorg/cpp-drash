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
#include "../misc/canimatedparam.h"

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
    CSceneObjectParams();

    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    bool mFixedRotation;
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
    typedef CFigure *CFigurePtr;
    typedef CFigurePtr FiguresT[mFiguresCountLimit];

    inline const b2Body* GetBody(void) const;

    inline CScene *GetScene();
    inline const CScene *GetScene() const;

    inline void SetDynamic( bool _dynamic );

    CFigurePtr CreateFigure( const CFigureParams &_params );
    void DestroyFigure(CFigure *_figure);
    inline const FiguresT &GetFigures();
    inline unsigned int EnumFigures() const;

    inline void ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos );
    inline void SetLinearVelocity( const CVec2 &_vel );
    inline void SetAngularVelocity( float _vel );
    inline void SetFixedRotation( bool _fixed );
    inline void SetActive( bool _active );

    void SetPos( const CVec2 &_pos );
    inline void SetPosTarget( const CVec2 &_target, double _time, const AnimationBehavior &_behavior );
    inline const CAnimatedParam<CVec2> &GetPos() const;
    void SetAngle( float _angle );
    inline void SetAngleTarget( float _target, double _time, const AnimationBehavior &_behavior );
    inline const CAnimatedParam<float> &GetAngle() const;

    inline void SetZ(float _z);
    inline void SetZTarget( float _target, double _time, const AnimationBehavior &_behavior );
    inline const CAnimatedParam<float> &GetZ() const;

    void DumpGeometry(GeometryT &_geometry) const;
    void DumpParams(ParamsT &_params) const;

    virtual void ComputeBoundingBox();
    const b2AABB &GetBoundingBox() const;

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
    b2Body* mBody;
    CScene* mScene;
    bool mDead = false;
    int mInternalId = 0;
    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;
    float mColor[3];
    FiguresT mFigures;
    unsigned int mFiguresCount = 0;
    CAnimatedParam<float> mZ;
};

inline const b2Body *CSceneObject::GetBody() const
{
    return mBody;
}

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

inline const CSceneObject::FiguresT &CSceneObject::GetFigures()
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

inline void CSceneObject::SetLinearVelocity( const CVec2 &_vel )
{
    mBody->SetLinearVelocity(_vel);
}

inline void CSceneObject::SetAngularVelocity( float _vel )
{
    mBody->SetAngularVelocity(_vel);
}

inline void CSceneObject::SetFixedRotation( bool _fixed )
{
    mBody->SetFixedRotation(_fixed);
}

inline void CSceneObject::SetActive( bool _active )
{
    mBody->SetActive(_active);
}

inline void CSceneObject::SetPosTarget( const CVec2 &_target, double _time, const AnimationBehavior &_behavior )
{
    mPos.SetTarget( _target, _time, _behavior );
}

inline const CAnimatedParam<CVec2> &CSceneObject::GetPos() const
{
    return mPos;
}

inline void CSceneObject::SetAngleTarget( float _target, double _time, const AnimationBehavior &_behavior )
{
    mAngle.SetTarget( _target, _time, _behavior );
}

inline const CAnimatedParam<float> &CSceneObject::GetAngle() const
{
    return mAngle;
}

inline void CSceneObject::SetZ(float _z)
{
    mZ.Set(_z);
}

inline void CSceneObject::SetZTarget(float _target, double _time, const AnimationBehavior &_behavior)
{
    mZ.SetTarget( _target, _time, _behavior );
}

inline const CAnimatedParam<float> &CSceneObject::GetZ() const
{
    return mZ;
}

} // namespace drash

#endif // CSCENEOBJECT_H
