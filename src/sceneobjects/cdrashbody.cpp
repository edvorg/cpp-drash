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

#include "../subsystem/templatesystem.h"

namespace drash
{

CDrashBody::CDrashBody()
{
}

bool CDrashBody::Init(const GeometryT &_geometry, const CDrashBody::ParamsT &_params )
{
    if ( CSceneObject::Init(_geometry, _params) == false )
    {
        return false;
    }

    mDestructionChilds = _geometry.mDestructionChilds;
    mDestroyDelay = _params.mDestroyDelay;
    mDestroySpeed = _params.mDestroySpeed;

    return true;
}

void CDrashBody::Step( double _dt )
{
    CSceneObject::Step(_dt);

    mTime += _dt;

    if ( mCounter == 1 )
    {
        mCounter++;

        for (auto i = mDestructionChilds.begin(); i != mDestructionChilds.end(); i++)
        {
            i->mParams.mPos.Set(GetWorldPoint(i->mParams.mPos.Vec2()), i->mParams.mPos.mZ + GetPos().Get().mZ);
            i->mParams.mAngle += GetAngle().Get();

            CDrashBody *db = GetScene()->CreateObject<CDrashBody>(*(i->mTemplate), i->mParams);

            if (db == nullptr)
            {
                break;
            }

            db->SetLinearVelocity(mLastVelocity);
            db->SetAngularVelocity(mLastAngularVelocity);
        }

		mDestructionChilds.clear();
        GetScene()->DestroyObject(this);
    }
}

void CDrashBody::OnContactBegin(const CFigure *_f1, const CFigure *_f2)
{
    CSceneObject::OnContactBegin(_f1, _f2);

    if (mCounter == 0 && mTime > mDestroyDelay && mDestructionChilds.size())
    {
        CVec2 vel = GetLinearVelocity();
        vel -= _f2->GetSceneObject()->GetLinearVelocity();

        if (vel.Length() >= mDestroySpeed)
        {
            mCounter++;
            mLastVelocity = GetLinearVelocity();
            mLastAngularVelocity = GetAngularVelocity();
        }
    }
}

} // namespace
