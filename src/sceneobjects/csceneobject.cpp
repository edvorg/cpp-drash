#include "csceneobject.h"
#include "../diag/clogger.h"

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
    mBody(NULL),
    mScene(NULL),
    mDead(false)
{
}

CSceneObject::~CSceneObject(void)
{
    Release();
}

void CSceneObject::BeginContact( const CContact &_contact )
{
}

void CSceneObject::PreSolve(const CContact &_contact)
{
}

void CSceneObject::PostSolve(const CContact &_contact)
{
}

void CSceneObject::EndContact( const CContact &_contact )
{
}

void CSceneObject::Boom(const CBoomParams &_boom)
{
}

bool CSceneObject::Init( const CSceneObject::ParamsT &_params )
{
    if ( mBody == NULL )
    {
        return false;
    }

    mPos.Set(_params.mPos);
    mAngle.Set(_params.mAngle);

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
    mPos.Step(_dt);
    mAngle.Step(_dt);

    if ( mBody->GetType() == b2_kinematicBody )
    {
        if ( mPos.IsTargetSet() || mAngle.IsTargetSet() )
        {
            mBody->SetTransform( mPos.Get(), mAngle.Get() );
        }
        else
        {
            mPos.Set( mBody->GetWorldCenter() );
            mAngle.Set( mBody->GetAngle() );
        }
    }
    else if ( mBody->GetType() == b2_dynamicBody )
    {
        mPos.Set( mBody->GetWorldCenter() );
        mAngle.Set( mBody->GetAngle() );
    }
}

b2Body *CSceneObject::GetBody()
{
    assert( mBody != NULL );

    return mBody;
}

const b2Body *CSceneObject::GetBody() const
{
    assert( mBody != NULL );

    return mBody;
}

CScene *CSceneObject::GetScene()
{
    return mScene;
}

const CScene *CSceneObject::GetScene() const
{
    return mScene;
}

void CSceneObject::SetDead()
{
    mDead = true;
}

bool CSceneObject::IsDead() const
{
    return mDead;
}

} // namespace drash
