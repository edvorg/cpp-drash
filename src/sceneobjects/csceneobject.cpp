#include "csceneobject.h"
#include "../clogger.h"
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
    mTargetSet(false),
    mTargetAngle(0),
    mAngleTargetSpeed(1),
    mTargetAngleSet(false),
    mScene(NULL),
    mDead(false)
{
    mTargetSpeed = CVec2(0,0);
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

void CSceneObject::SetTargetAngle(const float _angle)
{
    mTargetAngle = _angle;
    mTargetAngleSet = true;
}

void CSceneObject::SetAngleTargetSpeed(const float _speed)
{
    mAngleTargetSpeed = _speed;
}

void CSceneObject::RemoveTargetAngle()
{
    mTargetAngle = GetAngle();
    mTargetAngleSet = false;
}

void CSceneObject::SetAngle(float _angle)
{
    mBody->SetTransform( mBody->GetWorldCenter(), _angle );
}

float CSceneObject::GetAngle()
{
    return mBody->GetAngle();
}

void CSceneObject::BeginContact( const CContact &_contact )
{
}

void CSceneObject::EndContact( const CContact &_contact )
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
        dir *= (float)_dt;
        dir += GetPos();

        GetBody()->SetTransform( dir, GetAngle() );
        if (mTarget == GetPos()){
            mTargetSet = false;
        }
    }
    if ( mTargetAngleSet == true ){
        float curAngle = ((mTargetAngle - GetAngle()) * mAngleTargetSpeed * _dt) + GetAngle();
        this->SetAngle(curAngle);
        if ( fabs(curAngle - mTargetAngle) < 0.001 )
            RemoveTargetAngle();
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

CScene *CSceneObject::GetScene()
{
    return mScene;
}

const CScene *CSceneObject::GetScene() const
{
    return mScene;
}

void CSceneObject::SetDead(bool _dead)
{
    mDead = _dead;
}

bool CSceneObject::IsDead() const
{
    return mDead;
}

} // namespace drash
