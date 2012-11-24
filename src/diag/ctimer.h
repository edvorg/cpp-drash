#pragma once
#ifndef CTIMER_H
#define CTIMER_H

namespace drash
{

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
