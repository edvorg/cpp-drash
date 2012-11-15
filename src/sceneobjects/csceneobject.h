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

class CExplosionParams;

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
    friend class CScene;
    friend class CExplosion;

    typedef CSceneObjectGeometry GeometryT;
    typedef CSceneObjectParams ParamsT;

    const b2Body* GetBody(void) const;

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDynamic( bool _dynamic );

    void CreateFigure( const CFigureParams &_params );

    void ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos );
    void SetLinearVelocity( const CVec2 &_vel );
    void SetAngularVelocity( float _vel );
    void SetFixedRotation( bool _fixed );
    void SetActive( bool _active );

    void SetPos( const CVec2 &_pos );
    void SetPosTarget( const CVec2 &_target, double _time, const AnimationBehavior &_behavior );
    const CAnimatedParam<CVec2> &GetPos() const;
    void SetAngle( float _angle );
    void SetAngleTarget( float _target, double _time, const AnimationBehavior &_behavior );
    const CAnimatedParam<float> &GetAngle() const;

    void DumpGeometry(GeometryT &_geometry) const;
    void DumpParams(ParamsT &_params) const;

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

private:
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
