#include "cobjectcamera.h"


namespace drash
{

CCameraParams::CCameraParams():CSceneObjectParams()
{
}

// TODO: I had errors of compilation

//CObjectCamera::CObjectCamera(void)
//{
//}

bool CObjectCamera::Init(const ParamsT &_params)
{
    if ( !CSceneObject::Init(_params) ){
        return false;
    }

    return true;
}

void CObjectCamera::Release(void)
{
    CSceneObject::Release();
}



}// namespace drash
