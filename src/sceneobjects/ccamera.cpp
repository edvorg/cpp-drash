#include "ccamera.h"


namespace drash
{

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CCamera::CCamera():
    CSceneObject(),
    mZoom(1),
    mZoomTarget(1),
    mZoomTargetSpeed(1),
    mZoomTargetSet(false)
{
}

CCamera::~CCamera(void)
{
}

void CCamera::SetZoom(float _zoom)
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

float CCamera::GetZoom() const
{
    return mZoom;
}

void CCamera::SetZoomTarget( float _target )
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

void CCamera::SetZoomTargetSpeed( float _speed )
{
    mZoomTargetSpeed = _speed;
}

float CCamera::GetZoomTarget(void)
{
    return mZoomTarget;
}

float CCamera::GetZoomTargetSpeed(void)
{
    return mZoomTargetSpeed;
}

void CCamera::RemoveZoomTarget(void)
{
    mZoomTargetSet = false;
}

bool CCamera::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    GetBody()->SetType( b2_kinematicBody );

    return true;
}

void CCamera::Release(void)
{
    CSceneObject::Release();
}

void CCamera::Step(double _dt)
{
    CSceneObject::Step(_dt);

    if ( mZoomTargetSet == true )
    {
        mZoom += ( mZoomTarget - mZoom ) * mZoomTargetSpeed * _dt;
    }
}



}// namespace drash
