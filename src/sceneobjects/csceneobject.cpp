#include "csceneobject.h"
#include "../diag/clogger.h"
#include "cboom.h"

namespace drash
{

CSceneObjectParams::CSceneObjectParams():
    mDynamic(true),
    mPos(0),
    mAngle(0),
    mFixedRotation(false)
{
}

CFigureParams::CFigureParams():
    mFriction(1),
    mRestitution(0),
    mMass(1),
    mVertices()
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
    mBody->SetFixedRotation( _params.mFixedRotation );
    mBody->SetLinearDamping(0);
    mBody->SetGravityScale(1.0f);
    mBody->SetType( _params.mDynamic ?
                    b2_dynamicBody :
                    b2_kinematicBody );

    return true;
}

void CSceneObject::Release()
{
    while ( b2Fixture *f = mBody->GetFixtureList() )
    {
        delete reinterpret_cast<CInterval*>( f->GetUserData() );
        f->SetUserData(NULL);
        mBody->DestroyFixture(f);
    }
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

void CSceneObject::OnContactBegin( const CContact &_contact )
{
}

void CSceneObject::OnContactEnd( const CContact &_contact )
{
}

void CSceneObject::OnBoom( const CBoomParams &_boom )
{
}

const b2Body *CSceneObject::GetBody() const
{
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

void CSceneObject::SetDynamic( bool _dynamic )
{
    mBody->SetType( _dynamic ? b2_dynamicBody : b2_kinematicBody );
}

void CSceneObject::CreateFigure( const CFigureParams &_params )
{
    b2PolygonShape s;

    if ( _params.mVertices.size() == 0 )
    {
        s.SetAsBox( 1.0f, 1.0f );
    }
    else
    {
        s.Set( &*_params.mVertices.begin(), _params.mVertices.size() );
    }

    b2MassData md;
    s.ComputeMass( &md, 1.0 );

    b2FixtureDef fdef;
    fdef.density = _params.mMass / md.mass;
    fdef.friction = _params.mFriction;
    fdef.isSensor = false;
    fdef.restitution = _params.mRestitution;
    fdef.shape = &s;
    fdef.userData = NULL;

    b2Fixture *f = mBody->CreateFixture(&fdef);
    f->SetUserData( new CInterval( _params.mLayers ) );
}

void CSceneObject::ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos )
{
    mBody->ApplyLinearImpulse( _dir, _pos, true );
}

void CSceneObject::SetLinearVelocity( const CVec2 &_vel )
{
    mBody->SetLinearVelocity(_vel);
}

void CSceneObject::SetAngularVelocity( float _vel )
{
    mBody->SetAngularVelocity(_vel);
}

void CSceneObject::SetFixedRotation(bool _fixed)
{
    mBody->SetFixedRotation(_fixed);
}

void CSceneObject::SetBullet(bool _bullet)
{
    mBody->SetBullet(_bullet);
}

} // namespace drash
