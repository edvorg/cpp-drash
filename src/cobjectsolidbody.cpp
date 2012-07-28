#include "cobjectsolidbody.h"

namespace drash
{

CObjectSolidBody::CObjectSolidBody(void)
{
}

CObjectSolidBody::~CObjectSolidBody(void)
{
}

bool CObjectSolidBody::Init(const ParamsT &_params )
{
    b2MassData md;
    md.center.SetZero();
    md.I = 0.0f;
    md.mass = _params.mMass;

    b2PolygonShape s;
    s.SetAsBox( 1.0f, 1.0f );
    s.ComputeMass( &md, 1.0f );

    b2FixtureDef fdef;
    fdef.density = 1.0f;
    fdef.friction = _params.mFriction;
    fdef.isSensor = false;
    fdef.restitution = _params.mRestitution;
    fdef.shape = &s;
    fdef.userData = NULL;

    if ( mBody->CreateFixture(&fdef) == NULL )
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

void CObjectSolidBody::Release(void)
{
}

} // namespace drash
