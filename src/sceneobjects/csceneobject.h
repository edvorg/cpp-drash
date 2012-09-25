#pragma once
#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "../misc/cvec2.h"
#include "../misc/canimatedparam.h"

namespace drash
{

class CBoomParams;

class CScene;
class CSceneObject;

class CSceneObjectParams
{
public:
    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    bool mFixedRotation;
    CSceneObjectParams();
};

class CFigureParams
{
public:
    float mFriction;
    float mRestitution;
    float mMass;

    /// if size of mVertices is NULL, creates a box with (1, 1) dimentions
    std::vector<CVec2> mVertices;

    CFigureParams();
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

    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;

protected:    
    CSceneObject(void);
    virtual ~CSceneObject(void);

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnContactEnd( const CContact &_contact );
    virtual void OnBoom( const CBoomParams &_boom );

private:
    b2Body* mBody;
    CScene* mScene;
    bool mDead;
};

} // namespace drash

#endif // CSCENEOBJECT_H
