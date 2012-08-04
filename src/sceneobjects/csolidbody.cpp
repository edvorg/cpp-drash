#include "csolidbody.h"

namespace drash
{

CSolidBodyParams::CSolidBodyParams():
    mFriction(1.0f),
    mRestitution(0.0f),
    mMass(1.0f),
    mVertices(NULL),
    mVerticesCount(0)
{
}

CSolidBody::CSolidBody(void)
{
}

CSolidBody::~CSolidBody(void)
{
}

bool CSolidBody::Init(const ParamsT &_params )
{
    if ( CSceneObject::Init(_params) == false )
    {
        return false;
    }

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

} // namespace drash
