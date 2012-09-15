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

    CSceneObject(void);
    virtual ~CSceneObject(void);

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnContactEnd( const CContact &_contact );
    virtual void OnBoom( const CBoomParams &_boom );

    const b2Body* GetBody(void) const;
    b2Body* GetBody(void);

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDead();
    bool IsDead() const;

    CAnimatedParam<CVec2> mPos;
    CAnimatedParam<float> mAngle;

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

private:
    b2Body* mBody;
    CScene* mScene;
    bool mDead;
};

} // namespace drash

#endif // CSCENEOBJECT_H
