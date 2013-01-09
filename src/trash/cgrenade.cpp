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

#include "cgrenade.h"

#include "../cscene.h"

namespace drash
{

CGrenadeParams::CGrenadeParams():
    CBulletParams(),
    mTime(1.0)
{}

CGrenade::CGrenade():
    CBullet(),
    mTime(1.0),
    mTimer(),
    mCounter(0)
{
}

bool CGrenade::Init( const CGrenade::ParamsT &_params )
{
    if ( CBullet::Init(_params) == false )
    {
        return false;
    }

    mTime = _params.mTime;

    return true;
}

void CGrenade::OnContactBegin( const CContact &_contact )
{
    if ( mCounter == 0 )
    {
        mCounter++;
        mTimer.Reset(true);
    }
}

void CGrenade::Step( double _dt )
{
    CBullet::Step(_dt);

    if ( mCounter == 1 )
    {
        mTimer.Tick();

        if ( mTimer.GetFullTime() >= mTime )
        {
            mCounter++;
            CBoomParams boom;
            boom.mPos = GetPos().Get();
            boom.mStregth = 500;
            GetScene()->AddRequestBoom(boom);
            SetDead();
        }
    }
}


}// namespace drash
