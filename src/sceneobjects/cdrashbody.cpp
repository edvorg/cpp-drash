#include "cdrashbody.h"

#include "../clogger.h"

namespace drash
{

CDrashBody::CDrashBody():
    mStrips(),
    mDrashVertices()
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

    mDrashVertices = _params.mDrashVertices;
    mStrips = _params.mStrips;

    return true;
}

void CDrashBody::Release(void)
{
    CSolidBody::Release();
}

void CDrashBody::BeginContact(const CContact &_contact)
{
    CSolidBody::BeginContact(_contact);

    LOG_INFO("begin ");

    for ( unsigned int i=0; i<_contact.mPointCount; i++ )
    {
        LOG_INFO(_contact.mPoints[i]);
    }
}

void CDrashBody::EndContact(const CContact &_contact)
{
    CSolidBody::EndContact(_contact);

    LOG_INFO("end");
}

} // namespace
