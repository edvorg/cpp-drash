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

#pragma once
#ifndef CTIMER_H
#define CTIMER_H

namespace drash
{

class CLogger;

class CTimer
{
public:
    CTimer();

    /// if _start is true - automaticly starts timer
    void Reset( bool _start );
    /// updates full time and delta time
    void Tick();

    inline void SetPaused( bool _paused );
    inline bool IsPaused() const;

    /// time (seconds) from mStartTime
    double GetFullTime() const;

    /// time (seconds) spent between last two Tick() invokations
    double GetDeltaTime() const;

    friend CLogger &operator <<(CLogger &_logger, const CTimer &_timer);

private:
    void Update();

    unsigned long mStartTime = 0;
    unsigned long mPrevTime = 0;
    unsigned long mCurrTime = 0;
    bool mPaused = true;
};

inline void CTimer::SetPaused( bool _paused )
{
    mPaused = _paused;
}

inline bool CTimer::IsPaused() const
{
    return mPaused;
}

} // namespace drash

#endif // CTIMER_H
