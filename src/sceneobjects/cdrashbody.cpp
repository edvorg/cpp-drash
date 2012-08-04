#include "cdrashbody.h"

#include "../clogger.h"
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
    const unsigned int vc = 4;
    const CVec2 v[vc] =
    {
        CVec2( -5.0f, 5.0f ),
        CVec2( -5.0f, -5.0f ),
        CVec2( 5.0f, -5.0f ),
        CVec2( 5.0f, 5.0f )
    };

    CDrashBody::ParamsT p = _params;

    p.mVertices = v;
    p.mVerticesCount = vc;

    if ( CSolidBody::Init(p) == false )
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
    if (mDestroy == true)
    {
        mDestroy = false;

        const unsigned int vc = 4;
        const CVec2 v[vc] =
        {
            CVec2( -2.5f, 2.5f ),
            CVec2( -2.5f, -2.5f ),
            CVec2( 2.5f, -2.5f ),
            CVec2( 2.5f, 2.5f )
        };

        CSolidBodyParams p;
        p.mVertices = v;
        p.mVerticesCount = vc;
        p.mAngle = GetBody()->GetAngle();
        p.mRestitution = 0.2f;

        p.mPos = GetBody()->GetWorldPoint( CVec2(2.5f, 2.5f) );
        GetScene()->CreateObject<CSolidBody>(p)->GetBody()->SetLinearVelocity(mLastSpeed);
        p.mPos = GetBody()->GetWorldPoint( CVec2(2.5f, -2.5f) );
        GetScene()->CreateObject<CSolidBody>(p)->GetBody()->SetLinearVelocity(mLastSpeed);
        p.mPos = GetBody()->GetWorldPoint( CVec2(-2.5f, -2.5f) );
        GetScene()->CreateObject<CSolidBody>(p)->GetBody()->SetLinearVelocity(mLastSpeed);
        p.mPos = GetBody()->GetWorldPoint( CVec2(-2.5f, 2.5f) );
        GetScene()->CreateObject<CSolidBody>(p)->GetBody()->SetLinearVelocity(mLastSpeed);

        GetBody()->GetWorld()->DestroyBody(GetBody());
    }
}

} // namespace
