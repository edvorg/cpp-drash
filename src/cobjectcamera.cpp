#include "cobjectcamera.h"


namespace drash
{

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CObjectCamera::CObjectCamera():
    CSceneObject(),
    mTarget(0),
    mTargetSpeed(1),
    mTargetSet(false),
    mZoom(1)
{
}

CObjectCamera::~CObjectCamera(void)
{
}

bool CObjectCamera::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    mBody->SetType( b2_kinematicBody );

    return true;
}

void CObjectCamera::Release(void)
{
    CSceneObject::Release();
}

void CObjectCamera::Step(double _dt)
{
    if ( mTargetSet == true )
    {
        CVec2 dir = mTarget;
        dir -= mBody->GetWorldCenter();
        dir.x *= mTargetSpeed.x;
        dir.y *= mTargetSpeed.y;
        dir *= _dt;
        dir += mBody->GetWorldCenter();

        mBody->SetTransform( dir, mBody->GetAngle() );
    }
}

const CVec2 CObjectCamera::GetPos() const
{
    CVec2 res;
    res.Set(mBody->GetWorldCenter().x, mBody->GetWorldCenter().y );
    return res;
}

void CObjectCamera::SetTarget( const CVec2 &_target )
{
    mTarget = _target;
    mTargetSet = true;
}

void CObjectCamera::SetTargetSpeed( const CVec2 &_speed )
{
    mTargetSpeed = _speed;
}

const CVec2 &CObjectCamera::GetTarget() const
{
    return mTarget;
}

const CVec2 &CObjectCamera::GetTargetSpeed() const
{
    return mTargetSpeed;
}

void CObjectCamera::RemoveTarget()
{
    mTargetSet = false;
}

void CObjectCamera::SetZoom(float _zoom)
{
    if ( _zoom < 0.0f )
    {
        mZoom = 0;
    }
    else
    {
        mZoom = _zoom;
    }
}

float CObjectCamera::GetZoom() const
{
    return mZoom;
}



}// namespace drash
