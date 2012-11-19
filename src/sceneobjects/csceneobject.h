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

    static const unsigned int mFiguresCountLimit = 16;

    typedef CSceneObjectGeometry GeometryT;
    typedef CSceneObjectParams ParamsT;
    typedef CFigure *CFigurePtr;
    typedef CFigurePtr FiguresT[mFiguresCountLimit];

    const b2Body* GetBody(void) const;

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDynamic( bool _dynamic );

    CFigurePtr CreateFigure( const CFigureParams &_params );
    void DestroyFigure(CFigure *_figure);
    const FiguresT &GetFigures();
    unsigned int EnumFigures() const;

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
    int mInternalId;
    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;
    float mColor[3];
    FiguresT mFigures;
    unsigned int mFiguresCount = 0;
};

} // namespace drash

#endif // CSCENEOBJECT_H
