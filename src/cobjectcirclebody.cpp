#include "cobjectcirclebody.h"

namespace drash
{

CCircleBodyParams::CCircleBodyParams():
    mFriction(1.0f),
    mRestitution(0.0f),
    mMass(1.0f),
    mRadius(3.0f)
{
}

CObjectCircleBody::CObjectCircleBody()
{
}

CObjectCircleBody::~CObjectCircleBody()
{
}

bool CObjectCircleBody::Init(const CObjectCircleBody::ParamsT &_params)
{
    if ( CSceneObject::Init(_params) == false )
    {
        return false;
    }

    b2MassData md;
    md.center.SetZero();
    md.I = 0.0f;
    md.mass = _params.mMass;

    b2CircleShape s;
    s.m_radius = _params.mRadius;
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

void CObjectCircleBody::Release()
{
}

}
