#include "csolidbody.h"
#include "cboom.h"

namespace drash
{

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

    for ( auto i = _params.mFigures.begin(), i_e = _params.mFigures.end(); i != i_e; i++ )
    {
        CreateFigure(*i);
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

    ApplyLinearImpulse( dir, mPos.Get() );
}

} // namespace drash
