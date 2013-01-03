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
#include "../misc/animatedparam.h"
#include "../misc/vec3.h"
#include "../misc/color4.h"

namespace drash
{

class CScene;
class CFigureParams;
class CFigure;
class CExplosionParams;

class CSceneObjectGeometry
{
public:
    std::vector<CFigureParams> mFigures;
    std::vector<std::vector<unsigned int>> mDestructionGraph;

protected:
private:
};

class CSceneObjectParams
{
public:
    bool mDynamic = true;
    CVec3f mPos;
    float mAngle = 0;
    bool mFixedRotation = false;
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
    inline void ApplyLinearImpulse( const CVec2f &_dir, const CVec2f &_pos );
    inline void SetLinearVelocity(const CVec2f &_vel);
    inline CVec2f GetLinearVelocity() const;
    inline void SetAngularVelocity(float _vel);
    inline float GetAngularVelocity() const;
    inline void SetFixedRotation( bool _fixed );
    inline void SetActive( bool _active );
    inline CVec2f GetWorldPoint(const CVec2f &_local_point) const;
    inline CVec2f GetMassCenter() const;

    inline CAnimatedParam<CVec3f> &GetPos();
    inline CAnimatedParam<float> &GetAngle();

    virtual void ComputeBoundingBox();
    inline const b2AABB &GetBoundingBox() const;

    friend CLogger &operator <<(CLogger &_logger, const CSceneObject &_object);

    void DumpGeometry(CSceneObjectGeometry *_geometry) const;

    inline CColor4f &GetColor();
    inline const CColor4f &GetColor() const;
protected:
    CSceneObject(void);
    virtual ~CSceneObject(void);

    bool Init( const GeometryT &_geometry, const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

    virtual void OnContactBegin(const CFigure *_f1, const CFigure *_f2);
    virtual void OnContactPreSolve(const CFigure *, const CFigure *);
    virtual void OnContactEnd(const CFigure *, const CFigure *);
    virtual void OnBoom( const CExplosionParams &_boom );
    virtual void DrawDebug() const;

    b2AABB mBoundingBox;

private:
    b2Body* mBody = nullptr;
    CScene* mScene = nullptr;

    //////////////////////////////////////////////////////
    /// values, changed only be owning system (CScene) ///

    /// if flag is true, object will be destoyed with next CScene::Step() invokation
    bool mDead = false;

    /// id. just for perfomance
    int mInternalId = -1;

    //////////////////////////////////////////////
    /// figures factory //////////////////////////

    CFigure *mFigures[mFiguresCountLimit];
    unsigned int mFiguresCount = 0;

    /// world space postition in physics world
    CAnimatedParam<CVec3f> mPos;

    /// rotation angle in radians
    CAnimatedParam<float> mAngle;

    /// color, used for debug rendering. will be removed
    CColor4f mDebugColor;

    float mLifeTime = 0.0f;
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

inline void CSceneObject::ApplyLinearImpulse( const CVec2f &_dir, const CVec2f &_pos )
{
    mBody->ApplyLinearImpulse(CVec2ToB2Vec2(_dir), CVec2ToB2Vec2(_pos));
}

inline void CSceneObject::SetLinearVelocity(const CVec2f &_vel)
{
    mBody->SetLinearVelocity(CVec2ToB2Vec2(_vel));
}

inline CVec2f CSceneObject::GetLinearVelocity() const
{
    return B2Vec2ToCVec2(mBody->GetLinearVelocity());
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

inline CVec2f CSceneObject::GetWorldPoint(const CVec2f &_local_point) const
{
    return B2Vec2ToCVec2(mBody->GetWorldPoint(CVec2ToB2Vec2(_local_point)));
}

inline CVec2f CSceneObject::GetMassCenter() const
{
    return B2Vec2ToCVec2(mBody->GetWorldCenter());
}

inline CAnimatedParam<CVec3f> &CSceneObject::GetPos()
{
    return mPos;
}

inline CAnimatedParam<float> &CSceneObject::GetAngle()
{
    return mAngle;
}

inline const b2AABB &CSceneObject::GetBoundingBox() const
{
    return mBoundingBox;
}

inline CColor4f &CSceneObject::GetColor()
{
    return mDebugColor;
}

inline const CColor4f &CSceneObject::GetColor() const
{
    return mDebugColor;
}

} // namespace drash

#endif // CSCENEOBJECT_H
