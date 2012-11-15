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

    void SetPaused( bool _paused );
    bool IsPaused() const;

    /// time (seconds) from mStartTime
    double GetFullTime() const;

    /// time (seconds) spent between last two Tick() invokations
    double GetDeltaTime() const;

private:
    void Update();

    unsigned long mStartTime;
    unsigned long mPrevTime;
    unsigned long mCurrTime;
    bool mPaused;
};

} // namespace drash

#endif // CTIMER_H
