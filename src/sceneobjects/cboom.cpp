#include "cboom.h"

namespace drash
{

CBoom::CBoom():
    CSceneObject()
{
}

bool CBoom::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    SetDynamic(false);

    return true;
}

void CBoom::Release(void)
{
    CSceneObject::Release();
}

void CBoom::Step( double _dt )
{
    CSceneObject::Step(_dt);
}

}// namespace drash

