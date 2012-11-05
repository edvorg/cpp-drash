#include "cbullet.h"

#include "../cscene.h"

namespace drash
{

CBulletParams::CBulletParams():
    CSceneObjectParams(),
    mTarget(0,0)
{
}

CBullet::CBullet():
    CSceneObject()
{
}

bool CBullet::Init( const ParamsT &_params )
{
    if (CSceneObject::Init(_params) == false)
	{
        return false;
    }

    SetBullet(false);

    CVec2 v = _params.mTarget;
    v.x -= _params.mPos.x;
    v.y -= _params.mPos.y;
    v *= 30.0;
    SetLinearVelocity(v);
    return true;
}


void CBullet::OnContactBegin( const CContact &_contact )
{
    this->SetDead();
    CBoomParams boom;
    boom.mPos = GetPos().Get();
    boom.mStregth = 50000;
    GetScene()->AddRequestBoom(boom);
}

void CBullet::OnBoom( const CBoomParams &_boom )
{
}

}// namespace drash