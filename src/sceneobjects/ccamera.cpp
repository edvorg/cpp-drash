#include "ccamera.h"


namespace drash
{

const double CCamera::m_ZoomMax = 100.0;

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
    mZoom.Step(_dt);
}



}// namespace drash
