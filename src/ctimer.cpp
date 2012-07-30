#include "ctimer.h"
#include <stdlib.h>
#include <sys/time.h>

namespace drash
{

CTimer::CTimer():
    mStartTime(0),
    mPrevTime(0),
    mCurrTime(0)
{
}

void CTimer::Reset()
{
    this->Update();
    mStartTime = mPrevTime = mStartTime;
}

void CTimer::Tick()
{
    mPrevTime = mCurrTime;
    this->Update();
}

double CTimer::GetFullTime() const
{
    return ( mCurrTime - mStartTime ) / 1000000.0;
}

double CTimer::GetDeltaTime() const
{
    return ( mCurrTime - mPrevTime ) / 1000000.0;
}

void CTimer::Update()
{
    timeval t;
    gettimeofday( &t, NULL );
    mCurrTime = t.tv_sec * 1000000 + t.tv_usec;
}

} // namespace drash
