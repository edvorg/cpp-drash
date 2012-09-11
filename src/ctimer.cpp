#include "ctimer.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "diag/clogger.h"
#include <cassert>

namespace drash
{

CTimer::CTimer():
    mStartTime(0),
    mPrevTime(0),
    mCurrTime(0),
    mPaused(true)
{
}

void CTimer::Reset(bool _start)
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

void CTimer::SetPaused( bool _paused )
{
    mPaused = _paused;
}

bool CTimer::IsPaused() const
{
    return mPaused;
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

    assert( mCurrTime >= mPrevTime );
}

} // namespace drash
