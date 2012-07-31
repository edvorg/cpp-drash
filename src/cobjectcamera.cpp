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
    mZoom(1),
    mZoomTarget(1),
    mZoomTargetSpeed(1),
    mZoomTargetSet(false)
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

    if ( mZoomTargetSet == true )
    {
        mZoom += ( mZoomTarget - mZoom ) * mZoomTargetSpeed * _dt;
    }
}

void CObjectCamera::SetPosTarget( const CVec2 &_target )
{
    mTarget = _target;
    mTargetSet = true;
}

void CObjectCamera::SetPosTargetSpeed( const CVec2 &_speed )
{
    mTargetSpeed = _speed;
}

const CVec2 &CObjectCamera::GetPosTarget() const
{
    return mTarget;
}

const CVec2 &CObjectCamera::GetPosTargetSpeed() const
{
    return mTargetSpeed;
}

void CObjectCamera::RemovePosTarget()
{
    mTargetSet = false;
}

void CObjectCamera::SetZoom(float _zoom)
{
    if ( _zoom < 1.0f )
    {
        mZoom = 1;
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

void CObjectCamera::SetZoomTarget( float _target )
{
    if ( _target < 1.0f )
    {
        mZoomTarget = 1;
    }
    else
    {
        mZoomTarget = _target;
    }

    mZoomTargetSet = true;
}

void CObjectCamera::SetZoomTargetSpeed( float _speed )
{
    mZoomTargetSpeed = _speed;
}

float CObjectCamera::GetZoomTarget(void)
{
    return mZoomTarget;
}

float CObjectCamera::GetZoomTargetSpeed(void)
{
    return mZoomTargetSpeed;
}

void CObjectCamera::RemoveZoomTarget(void)
{
    mZoomTargetSet = false;
}



}// namespace drash
