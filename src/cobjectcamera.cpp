#include "cobjectcamera.h"


namespace drash
{

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CObjectCamera::CObjectCamera():
    CSceneObject(),
    mZoom(1),
    mZoomTarget(1),
    mZoomTargetSpeed(1),
    mZoomTargetSet(false)
{
}

CObjectCamera::~CObjectCamera(void)
{
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

bool CObjectCamera::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    GetBody()->SetType( b2_kinematicBody );

    return true;
}

void CObjectCamera::Release(void)
{
    CSceneObject::Release();
}

void CObjectCamera::Step(double _dt)
{
    CSceneObject::Step(_dt);

    if ( mZoomTargetSet == true )
    {
        mZoom += ( mZoomTarget - mZoom ) * mZoomTargetSpeed * _dt;
    }
}



}// namespace drash
