// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "timer.h"

#include <sys/time.h>
#include "assert.h"

#if defined(__MACH__)
#include <mach/clock.h>
#include <mach/mach.h>
#endif

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

CLogger &operator <<(CLogger &_logger, const CTimer &_timer)
{
    _logger<<"t: "<<_timer.GetFullTime()<<" dt: "<<_timer.GetDeltaTime();
    return _logger;
}

void CTimer::Update()
{
    timespec ts;

    #if defined(__MACH__) // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
    #else
    clock_gettime(CLOCK_REALTIME, &ts);
    #endif

    mCurrTime = ( ts.tv_sec * 1000000000 + ts.tv_nsec );

    DRASH_ASSERT( mCurrTime >= mPrevTime &&
                  "CTimer::Update(): something wrong with time query library" );
}

} // namespace drash
