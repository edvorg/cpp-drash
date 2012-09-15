#include "ccamera.h"


namespace drash
{

const float CCamera::m_ZoomMax = 100.0f;

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CCamera::CCamera():
    CSceneObject()
{
    mZoom.Set(1);
}

CCamera::~CCamera(void)
{
}

bool CCamera::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    GetBody()->SetType(b2_kinematicBody);

    return true;
}

void CCamera::Release(void)
{
    CSceneObject::Release();
}

void CCamera::Step(double _dt)
{
    CSceneObject::Step(_dt);
    mZoom.Step(_dt);
}

double CCamera::GetZoom() const
{
    return mZoom.Get();
}

double CCamera::GetZoomTarget() const
{
    return mZoom.GetTarget();
}

void CCamera::SetZoomTarget( float _target, double _time )
{
    mZoom.SetTarget( std::min( std::max( 1.0f, _target ), m_ZoomMax ),
                     _time,
                     AnimationBehaviorSingle );
}

}// namespace drash
