#pragma once
#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "../misc/cvec2.h"
#include "../misc/canimatedparam.h"
#include "../misc/math.h"
#include <vector>

namespace drash
{

class CBoomParams;

class CScene;
class CSceneObject;

class CFigureParams
{
public:
    CFigureParams();

    float mFriction;
    float mRestitution;
    float mMass;

    /// if size of mVertices is NULL, creates a box with (1, 1) dimentions
    std::vector<CVec2> mVertices;

    CInterval mLayers;
};

class CSceneObjectParams
{
public:
    CSceneObjectParams();

    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    bool mFixedRotation;
    std::vector<CFigureParams> mFigures;
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
    friend class CScene;
    friend class CExplosion;

    typedef CSceneObjectParams ParamsT;

    const b2Body* GetBody(void) const;

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDead();
    bool IsDead() const;

    void SetDynamic( bool _dynamic );

    void CreateFigure( const CFigureParams &_params );

    void ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos );
    void SetLinearVelocity( const CVec2 &_vel );
    void SetAngularVelocity( float _vel );
    void SetFixedRotation( bool _fixed );
    void SetBullet( bool _bullet );
    void SetActive( bool _active );

    void SetPos( const CVec2 &_pos );
    void SetPosTarget( const CVec2 &_target, double _time, const AnimationBehavior &_behavior );
    const CAnimatedParam<CVec2> &GetPos() const;
    void SetAngle( float _angle );
    void SetAngleTarget( float _target, double _time, const AnimationBehavior &_behavior );
    const CAnimatedParam<float> &GetAngle() const;

protected:    
    CSceneObject(void);
    virtual ~CSceneObject(void);

    bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnContactPreSolve( const CContact &_contact );
    virtual void OnContactEnd( const CContact &_contact );
    virtual void OnBoom( const CBoomParams &_boom );
    virtual void DrawDebug() const;

private:
    void DrawSide( const CVec2 &_v1, const CVec2 &_v2, const CInterval &_interval, const b2Color &_diffuse ) const;

    b2Body* mBody;
    CScene* mScene;
    bool mDead;
    std::vector<CInterval> mLayers;
    int mInternalId;
    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;
    float mColor[3];
};

} // namespace drash

#endif // CSCENEOBJECT_H
