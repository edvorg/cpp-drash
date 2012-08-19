#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "../cvec2.h"
#include "../ccontactlistener.h"
#include "../cboomparams.h"
#include "../canimatedparam.h"

namespace drash
{

class CScene;

class CSceneObjectParams
{
public:
    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    bool mFixedRotation;
    CSceneObjectParams();
};

class CSceneObject
{
public:
    friend class CScene;

    typedef CSceneObjectParams ParamsT;

    CSceneObject(void);
    virtual ~CSceneObject(void);

    virtual void BeginContact( const CContact &_contact );
    virtual void PreSolve(const CContact &_contact);
    virtual void PostSolve(const CContact &_contact);
    virtual void EndContact( const CContact &_contact );

    virtual void Boom(const CBoomParams &_boom);
    const b2Body* GetBody(void) const;
    b2Body* GetBody(void);

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDead(bool _dead);
    bool IsDead()const;

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
