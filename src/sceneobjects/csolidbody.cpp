#include "csolidbody.h"
#include "cboom.h"

namespace drash
{

CSolidBodyParams::CSolidBodyParams():
    mFriction(1.0f),
    mRestitution(0.0f),
    mMass(1.0f),
    mVertices()
{
}

CSolidBody::CSolidBody(void)
{
}

CSolidBody::~CSolidBody(void)
{
}

bool CSolidBody::Init( const ParamsT &_params )
{
    if ( CSceneObject::Init(_params) == false )
    {
        return false;
    }

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

    if ( GetBody()->CreateFixture(&fdef) == NULL )
	{
		return false;
    }

	return true;
}

void CSolidBody::Release(void)
{
    CSceneObject::Release();
}

void CSolidBody::OnBoom( const CBoomParams &_boom )
{
    CVec2 dir( GetBody()->GetWorldCenter() );
    dir -= _boom.mPos;

    float k = std::min( dir.Length(), _boom.mStregth )/ _boom.mStregth;

    dir.Normalize();
    dir *= k * _boom.mStregth;

    GetBody()->ApplyLinearImpulse( dir, mPos.Get() );
}

} // namespace drash
