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

namespace drash {

    class CLogger;

    class CTimer {
      public:
        CTimer();

        /// if _start is true - automaticly starts timer
        void Reset(bool _start);
        /// updates full time and delta time
        void Tick();

        inline void SetPaused(bool _paused);
        inline bool IsPaused() const;

        /// time (seconds) from startTime
        double GetFullTime() const;

        /// time (seconds) spent between last two Tick() invokations
        double GetDeltaTime() const;

        friend CLogger& operator<<(CLogger& _logger, const CTimer& _timer);

      private:
        void Update();

        unsigned long startTime = 0;
        unsigned long prevTime = 0;
        unsigned long currTime = 0;
        bool paused = true;
    };

    inline void CTimer::SetPaused(bool _paused) { paused = _paused; }

    inline bool CTimer::IsPaused() const { return paused; }

} // namespace drash

#endif // CTIMER_H
