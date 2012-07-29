#include "csceneobject.h"

namespace drash
{

CSceneObjectParams::CSceneObjectParams():
    mDynamic(true),
    mPos(0),
    mAngle(0)
{
}

CSceneObject::CSceneObject(void):
    mBody(NULL)
{
}

CSceneObject::~CSceneObject(void)
{
}

bool CSceneObject::Init(const CSceneObject::ParamsT &_params)
{
    if ( mBody == NULL )
    {
        return false;
    }

    mBody->SetTransform( _params.mPos, _params.mAngle );
    mBody->SetActive(true);
    mBody->SetAwake(true);
    mBody->SetSleepingAllowed(true);
    mBody->SetUserData(this);
    mBody->SetType( _params.mDynamic ?
                        b2_dynamicBody :
                        b2_staticBody );

    return true;
}

void CSceneObject::Release()
{
}

} // namespace drash
