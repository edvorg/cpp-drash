#include "cboom.h"

namespace drash
{

CBoom::CBoom():
    CSceneObject()
{
}

CBoom::~CBoom(void)
{
}

bool CBoom::Init( const ParamsT &_params )
{
    if ( !CSceneObject::Init(_params) )
    {
        return false;
    }

    GetBody()->SetType( b2_kinematicBody );

    return true;
}

void CBoom::Release(void)
{
    CSceneObject::Release();
}

void CBoom::Step(double _dt)
{
    CSceneObject::Step(_dt);
}

}// namespace drash

