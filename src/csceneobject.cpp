#include "csceneobject.h"

namespace drash
{

CSceneObjectParams::CSceneObjectParams():
    mDynamic(true),
    mPos(0),
    mAngle(0),
    mFixedRotation(false)
{
}

CSceneObject::CSceneObject(void):
    mBody(NULL),
    mTarget(0),
    mTargetSpeed(1),
    mTargetSet(false)
{
}

CSceneObject::~CSceneObject(void)
{
}

void CSceneObject::SetPos( const CVec2 &_pos )
{
    mBody->SetTransform( _pos, mBody->GetAngle() );
}

const CVec2 CSceneObject::GetPos() const
{
    CVec2 res;
    res.Set(mBody->GetWorldCenter().x, mBody->GetWorldCenter().y );
    return res;
}

void CSceneObject::SetPosTarget( const CVec2 &_target )
{
    mTarget = _target;
    mTargetSet = true;
}

void CSceneObject::SetPosTargetSpeed( const CVec2 &_speed )
{
    mTargetSpeed = _speed;
}

const CVec2 &CSceneObject::GetPosTarget() const
{
    return mTarget;
}

const CVec2 &CSceneObject::GetPosTargetSpeed() const
{
    return mTargetSpeed;
}

void CSceneObject::RemovePosTarget()
{
    mTargetSet = false;
}

void CSceneObject::SetAngle(float _angle)
{
    mBody->SetTransform( mBody->GetWorldCenter(), _angle );
}

float CSceneObject::GetAngle()
{
    return mBody->GetAngle();
}

void CSceneObject::BeginContact( CSceneObject *_object )
{
}

void CSceneObject::EndContact( CSceneObject *_object )
{
}

bool CSceneObject::Init( const CSceneObject::ParamsT &_params )
{
    if ( mBody == NULL )
    {
        return false;
    }

    mBody->SetTransform( _params.mPos, _params.mAngle );
    mBody->SetActive(true);
    mBody->SetAwake(true);
    mBody->SetSleepingAllowed(true);
    mBody->SetUserData(this);
    mBody->SetAngularDamping(0);
    mBody->SetBullet(false);
    mBody->SetFixedRotation(_params.mFixedRotation);
    mBody->SetLinearDamping(0);
    mBody->SetGravityScale(1.0f);
    mBody->SetType( _params.mDynamic ?
                        b2_dynamicBody :
                        b2_kinematicBody );

    return true;
}

void CSceneObject::Release()
{
}

void CSceneObject::Step( double _dt )
{
    if ( mTargetSet == true )
    {
        CVec2 dir = mTarget;
        dir -= GetPos();
        dir.x *= mTargetSpeed.x;
        dir.y *= mTargetSpeed.y;
        dir *= _dt;
        dir += GetPos();

        GetBody()->SetTransform( dir, GetAngle() );
    }
}

b2Body *CSceneObject::GetBody()
{
    assert( mBody != NULL );

    return mBody;
}

const b2Body *CSceneObject::GetBody() const
{
    assert( mBody != NULL );

    return mBody;
}

} // namespace drash
