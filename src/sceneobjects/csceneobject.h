#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "../cvec2.h"
#include "../ccontactlistener.h"

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

    void SetPos( const CVec2 &_pos );
    const CVec2 GetPos() const;

    void SetPosTarget( const CVec2 &_target );
    void SetPosTargetSpeed( const CVec2 &_speed );
    const CVec2 &GetPosTarget() const;
    const CVec2 &GetPosTargetSpeed() const;
    void RemovePosTarget();
    void SetTargetAngle(const float _angle);
    void SetAngleTargetSpeed(const float _speed);
    void RemoveTargetAngle();
    void SetAngle( float _angle );
    float GetAngle(void);

    virtual void BeginContact( const CContact &_contact );
    virtual void EndContact( const CContact &_contact );

    const b2Body* GetBody(void) const;
    b2Body* GetBody(void);

    CScene *GetScene();
    const CScene *GetScene() const;

    void SetDead(bool _dead);
    bool IsDead()const;

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

private:


    b2Body* mBody;

    CVec2 mTarget;
    CVec2 mTargetSpeed;
    bool mTargetSet;

    float mTargetAngle;
    float mAngleTargetSpeed;
    bool mTargetAngleSet;

    CScene* mScene;

    bool mDead;
};

} // namespace drash

#endif // CSCENEOBJECT_H
