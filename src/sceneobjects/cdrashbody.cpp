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

bool CDrashBody::Init( const CDrashBody::ParamsT &_params )
{
    if ( CSceneObject::Init(_params) == false )
    {
        return false;
    }

    mParams = _params;
    mTimer.Reset(true);

    return true;
}

void CDrashBody::Step( double _dt )
{
    CSceneObject::Step(_dt);

    mTimer.Tick();

    if ( mCounter == 1 )
    {
        mCounter++;

        for ( auto i = mParams.mChilds.begin(); i != mParams.mChilds.end(); i++ )
        {
            i->mPos = GetBody()->GetWorldPoint(i->mLocalPos);
            i->mAngle = GetBody()->GetAngle();

            CSceneObject* o = NULL;

            if ( i->mChilds.size() )
            {
                o = GetScene()->CreateObject<CDrashBody>(*i);  
            }
            else
            {
                o = GetScene()->CreateObject<CSceneObject>(*i);   
            }              

            if ( o == NULL )
            {
                break;
            }

            o->SetLinearVelocity(mLastVelocity);
            o->SetAngularVelocity(mLastAngularVelocity);
        }

        mBoomParams.clear();

        SetDead();
    }
}

void CDrashBody::OnContactBegin( const CContact &_contact )
{
    CSceneObject::OnContactBegin(_contact);

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

void CDrashBody::OnBoom( const CBoomParams &_boom )
{
    CSceneObject::OnBoom(_boom);

    if ( mCounter == 0 && mTimer.GetFullTime() > mParams.mDestroyDelay )
    {
        CVec2 dist = _boom.mPos;
        dist -= GetBody()->GetWorldCenter();

        if ( dist.Length() < _boom.mStregth )
        {
            mCounter++;
            mLastVelocity = GetBody()->GetLinearVelocity();
            mLastAngularVelocity = GetBody()->GetAngularVelocity();
            mBoomParams.push_back(_boom);
        }
    }
}

} // namespace
