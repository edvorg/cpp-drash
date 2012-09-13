#pragma once
#ifndef CTIMER_H
#define CTIMER_H

namespace drash
{

class CTimer
{
public:
    CTimer();

    void Reset( bool _start );
    void Tick();

    void SetPaused( bool _paused );
    bool IsPaused() const;

    /// time from mStartTime (in seconds)
    double GetFullTime() const;

    /// time from last tick (in seconds)
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
