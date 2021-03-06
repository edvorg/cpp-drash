// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

namespace drash {

    Timer::Timer() : startTime(0), prevTime(0), currTime(0), paused(true) {}

    void Timer::Reset(bool _start) {
        this->Update();
        startTime = prevTime = currTime;

        SetPaused(!_start);
    }

    void Timer::Tick() {
        if (paused == false) {
            prevTime = currTime;
            this->Update();
        } else {
            this->Update();
            prevTime = currTime;
        }
    }

    double Timer::GetFullTime() const {
        return (currTime - startTime) / 1000000000.0;
    }

    double Timer::GetDeltaTime() const {
        return (currTime - prevTime) / 1000000000.0;
    }

    Logger& operator<<(Logger& _logger, const Timer& _timer) {
        _logger << "t: " << _timer.GetFullTime()
                << " dt: " << _timer.GetDeltaTime();
        return _logger;
    }

    void Timer::Update() {
        timespec ts;

#if defined(__MACH__) // OS X does not have clock_gettime, use clock_get_time
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), ALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts.tv_sec = mts.tv_sec;
        ts.tv_nsec = mts.tv_nsec;
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif

        currTime = (ts.tv_sec * 1000000000 + ts.tv_nsec);

        DRASH_ASSERT(
            currTime >= prevTime &&
            "Timer::Update(): something wrong with time query library");
    }

} // namespace drash
