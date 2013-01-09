// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
