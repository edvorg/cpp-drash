#include "cdrashbody.h"

#include "../diag/clogger.h"
#include "../cscene.h"

namespace drash
{

void CDrashBody::BeginContact(const CContact &_contact)
{
    CSolidBody::BeginContact(_contact);

    mDestroy = true;
    mLastVelocity = GetBody()->GetLinearVelocity();
    mLastAngularVelocity = GetBody()->GetAngularVelocity();
}

void CDrashBody::EndContact(const CContact &_contact)
{
    CSolidBody::EndContact(_contact);
}

CDrashBody::CDrashBody():
    mDestroy(false),
    mLastVelocity(0),
    mLastAngularVelocity(0),
    mParams()
{
}

CDrashBody::~CDrashBody()
{
}

bool CDrashBody::Init(const CDrashBody::ParamsT &_params)
{
    if ( CSolidBody::Init(_params) == false )
    {
        return false;
    }

    mParams = _params;

    return true;
}

void CDrashBody::Release(void)
{
    CSolidBody::Release();
}

void CDrashBody::Step(double _dt)
{
    CSolidBody::Step(_dt);

    if ( mDestroy )
    {
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
        mDestroy = false;
    }
}

} // namespace
