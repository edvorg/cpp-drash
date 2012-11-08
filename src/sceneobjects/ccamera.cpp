#include "ccamera.h"
#include "explosion.h"

namespace drash
{

const float CCamera::m_ZoomMax = 500.0f;

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CCamera::CCamera():
    CSceneObject()
{
    mZoom.Set(1);
}

bool CCamera::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    SetDynamic(false);

    return true;
}

void CCamera::Step( double _dt )
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
