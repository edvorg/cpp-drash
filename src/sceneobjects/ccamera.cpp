#include "ccamera.h"
#include "explosion.h"

namespace drash
{

CCameraParams::CCameraParams():
    CSceneObjectParams()
{
}

CCamera::CCamera():
    CSceneObject()
{
    SetZ(100);
}

bool CCamera::Init(const GeometryT &_geometry, const ParamsT &_params )
{
    if ( !CSceneObject::Init(_geometry, _params) )
    {
        return false;
    }

    SetDynamic(false);

    return true;
}

}// namespace drash
