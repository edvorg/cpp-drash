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
    mParams()
{
    mTimer.Reset(false);
}

CDrashBody::~CDrashBody()
{
}

bool CDrashBody::Init( const CDrashBody::ParamsT &_params )
{
    if ( CSolidBody::Init(_params) == false )
    {
        return false;
    }

    mParams = _params;
    mTimer.SetPaused(false);

    return true;
}

void CDrashBody::OnContactBegin( const CContact &_contact )
{
    CSolidBody::OnContactBegin(_contact);

    if ( mCounter == 0 && mTimer.GetFullTime() > mParams.mDestroyDelay )
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

void CDrashBody::Release(void)
{
    CSolidBody::Release();
}

void CDrashBody::Step( double _dt )
{
    CSolidBody::Step(_dt);

    mTimer.Tick();

    if ( mCounter == 1 )
    {
        mCounter++;

        for ( auto i = mParams.mChilds.begin(); i != mParams.mChilds.end(); i++ )
        {
            i->mPos = GetBody()->GetWorldPoint(i->mLocalPos);
            i->mAngle = GetBody()->GetAngle();

            if ( i->mChilds.size() )
            {
                CSceneObject* o = GetScene()->CreateObject<CDrashBody>(*i);
                o->GetBody()->SetLinearVelocity(mLastVelocity);
                o->GetBody()->SetAngularVelocity(mLastAngularVelocity);
            }
            else
            {
                CSceneObject* o = GetScene()->CreateObject<CSolidBody>(*i);
                o->GetBody()->SetLinearVelocity(mLastVelocity);
                o->GetBody()->SetAngularVelocity(mLastAngularVelocity);
            }
        }

        SetDead();
    }
}

} // namespace
