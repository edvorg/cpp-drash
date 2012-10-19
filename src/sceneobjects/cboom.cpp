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

}// namespace drash

