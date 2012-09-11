#include "cdrashbody.h"

#include "../diag/clogger.h"
#include "../cscene.h"

namespace drash
{

void CDrashBody::BeginContact(const CContact &_contact)
{
    CSolidBody::BeginContact(_contact);

    LOG_INFO("begin ");

    for ( unsigned int i=0; i<_contact.mPointCount; i++ )
    {
        LOG_INFO(_contact.mPoints[i]);
    }

    mDestroy = true;
    mLastSpeed = GetBody()->GetLinearVelocity();
}

void CDrashBody::EndContact(const CContact &_contact)
{
    CSolidBody::EndContact(_contact);

    LOG_INFO("end");
}

CDrashBody::CDrashBody():
    mDestroy(false)
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

    return true;
}

void CDrashBody::Release(void)
{
    CSolidBody::Release();
}

void CDrashBody::Step(double _dt)
{
    CSolidBody::Step(_dt);
}

} // namespace
