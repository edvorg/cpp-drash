#ifndef CTIMER_H
#define CTIMER_H

class CTimer
{
public:
    CTimer();

    void Reset();
    void Tick();

    /// time from mStartTime (in seconds)
    double GetFullTime() const;

    /// time from last tick (in seconds)
    double GetDeltaTime() const;

private:
    void Update();

    unsigned long mStartTime;
    unsigned long mPrevTime;
    unsigned long mCurrTime;
};

#endif // CTIMER_H
