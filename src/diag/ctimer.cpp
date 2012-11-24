#include "ctimer.h"

#include <sys/time.h>
#include "assert.h"

namespace drash
{

CTimer::CTimer():
    mStartTime(0),
    mPrevTime(0),
    mCurrTime(0),
    mPaused(true)
{
}

void CTimer::Reset( bool _start )
{
    this->Update();
    mStartTime = mPrevTime = mCurrTime;

    SetPaused(!_start);
}

void CTimer::Tick()
{
    if ( mPaused == false )
    {
        mPrevTime = mCurrTime;
        this->Update();
    }
    else
    {
        this->Update();
        mPrevTime = mCurrTime;
    }
}

double CTimer::GetFullTime() const
{
    return ( mCurrTime - mStartTime ) / 1000000000.0;
}

double CTimer::GetDeltaTime() const
{
    return ( mCurrTime - mPrevTime ) / 1000000000.0;
}

void CTimer::Update()
{
    timespec ts;
    clock_gettime( CLOCK_REALTIME, &ts );
    mCurrTime = ( ts.tv_sec * 1000000000 + ts.tv_nsec );

    DRASH_ASSERT( mCurrTime >= mPrevTime &&
                  "CTimer::Update(): something wrong with time query library" );
}

} // namespace drash
