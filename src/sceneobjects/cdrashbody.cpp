// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "cdrashbody.h"

#include "../diag/clogger.h"
#include "../cscene.h"

namespace drash
{

CDrashBodyParams::CDrashBodyParams():
    mChilds(),
    mLocalPos(0),
    mDestroyDelay(0),
    mDestroySpeed(0)
{
}

CDrashBody::CDrashBody():
    mCounter(0),
    mLastVelocity(0),
    mLastAngularVelocity(0),
    mParams(),
    mBoomParams()
{
}

bool CDrashBody::Init(const GeometryT &_geometry, const CDrashBody::ParamsT &_params )
{
    if ( CSceneObject::Init(_geometry, _params) == false )
    {
        return false;
    }

    mParams = _params;

    return true;
}

void CDrashBody::Step( double _dt )
{
    CSceneObject::Step(_dt);

//    mTimer.Tick();

//    if ( mCounter == 1 )
//    {
//        mCounter++;

//        for ( auto i = mParams.mChilds.begin(); i != mParams.mChilds.end(); i++ )
//        {
//            i->mPos = GetBody()->GetWorldPoint(i->mLocalPos);
//            i->mAngle = GetBody()->GetAngle();

//            CSceneObject* o = NULL;

//            if ( i->mChilds.size() )
//            {
//                o = GetScene()->CreateObject<CDrashBody>(*i);
//            }
//            else
//            {
//                o = GetScene()->CreateObject<CSceneObject>(*i);
//            }

//            if ( o == NULL )
//            {
//                break;
//            }

//            o->SetLinearVelocity(mLastVelocity);
//            o->SetAngularVelocity(mLastAngularVelocity);
//        }

//        mBoomParams.clear();

//        SetDead();
//    }
}

void CDrashBody::OnContactBegin( const CContact &_contact )
{
    CSceneObject::OnContactBegin(_contact);

    if ( mCounter == 0 && mTime > mParams.mDestroyDelay )
    {
        CVec2 vel = GetBody()->GetLinearVelocity();
        vel -= _contact.obj->GetBody()->GetLinearVelocity();

        if ( vel.Length() > mParams.mDestroySpeed )
        {
            mCounter++;
            mLastVelocity = GetBody()->GetLinearVelocity();
            mLastAngularVelocity = GetBody()->GetAngularVelocity();
        }
    }
}

} // namespace
