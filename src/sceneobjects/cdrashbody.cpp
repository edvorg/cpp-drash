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

void CDrashBody::BeginContact(CSceneObject *_object)
{
    CSolidBody::BeginContact(_object);

    LOG_INFO("begin");
}

void CDrashBody::EndContact(CSceneObject *_object)
{
    CSolidBody::EndContact(_object);

    LOG_INFO("end");
}

} // namespace
