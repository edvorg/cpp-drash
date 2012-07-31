#include "csceneobject.h"

namespace drash
{

CSceneObjectParams::CSceneObjectParams():
    mDynamic(true),
    mPos(0),
    mAngle(0),
    mFixedRotation(false)
{
}

CSceneObject::CSceneObject(void):
    mBody(NULL)
{
}

CSceneObject::~CSceneObject(void)
{
}

bool CSceneObject::Init( const CSceneObject::ParamsT &_params )
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
    mBody->SetAngularDamping(0);
    mBody->SetBullet(false);
    mBody->SetFixedRotation(_params.mFixedRotation);
    mBody->SetLinearDamping(0);
    mBody->SetGravityScale(1.0f);
    mBody->SetType( _params.mDynamic ?
                        b2_dynamicBody :
                        b2_kinematicBody );

    return true;
}

void CSceneObject::Release()
{
}

void CSceneObject::Step( double _dt )
{
}

void CSceneObject::SetPos( const CVec2 &_pos )
{
    mBody->SetTransform( _pos, mBody->GetAngle() );
}

const CVec2 CSceneObject::GetPos() const
{
    CVec2 res;
    res.Set(mBody->GetWorldCenter().x, mBody->GetWorldCenter().y );
    return res;
}

} // namespace drash
