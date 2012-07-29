#include "cobjectsolidbody.h"

namespace drash
{

CSolidBodyParams::CSolidBodyParams():
    mDynamic(true),
    mPos(0),
    mAngle(0),
    mFriction(1.0f),
    mRestitution(0.0f),
    mMass(1.0f),
    mVertices(NULL),
    mVerticesCount(0)
{
}

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

    if ( _params.mVertices == NULL )
    {
        s.SetAsBox( 1.0f, 1.0f );
    }
    else
    {
        s.Set( _params.mVertices, _params.mVerticesCount );
    }

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
