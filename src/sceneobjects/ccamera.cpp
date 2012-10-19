#include "ccamera.h"
#include "cboom.h"

namespace drash
{

const float CCamera::m_ZoomMax = 500.0f;

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CCamera::CCamera():
    CSceneObject(),
    mShake(0)
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

    if ( mPos.IsTargetSet() == false )
    {
        mShake = 0;
    }
}

void CCamera::OnBoom( const CBoomParams &_boom )
{
    CSceneObject::OnBoom(_boom);

    if ( mShake == 0 )
    {
        mLastPos = GetBody()->GetWorldCenter();
    }

    CVec2 tmp = GetBody()->GetWorldCenter();
    tmp.y += 0.1 * _boom.mStregth / mZoom.Get();
    mPos.Set( tmp );
    mPos.SetTarget( mLastPos,
                    0.2,
                    AnimationBehaviorSingle );

    mShake++;
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
