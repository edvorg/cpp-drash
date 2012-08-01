#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "cvec2.h"

namespace drash
{

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
    void SetAngle( float _angle );
    float GetAngle(void);

    virtual void BeginContact( CSceneObject* _object );
    virtual void EndContact( CSceneObject* _object );

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    b2Body* GetBody(void);
    const b2Body* GetBody(void) const;

private:
    b2Body* mBody;

    CVec2 mTarget;
    CVec2 mTargetSpeed;
    bool mTargetSet;
    bool mTargetAngleSet;

    float mAngleTargetSpeed;
    float mTargetAngle;
};

} // namespace drash

#endif // CSCENEOBJECT_H
