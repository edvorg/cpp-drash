#include "cdrashbody.h"

namespace drash
{

CDrashBody::CDrashBody()
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

void CDrashBody::BeginContact(CSceneObject *_object)
{
    CSolidBody::BeginContact(_object);
}

void CDrashBody::EndContact(CSceneObject *_object)
{
    CSolidBody::EndContact(_object);
}

} // namespace
