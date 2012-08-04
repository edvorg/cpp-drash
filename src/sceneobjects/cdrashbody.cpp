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

void CDrashBody::BeginContact(CSceneObject *_object, const b2Manifold *_manifold)
{
    CSolidBody::BeginContact(_object, _manifold);

    LOG_INFO("begin");
}

void CDrashBody::EndContact(CSceneObject *_object, const b2Manifold *_manifold)
{
    CSolidBody::EndContact(_object, _manifold);

    LOG_INFO("end");
}

} // namespace
