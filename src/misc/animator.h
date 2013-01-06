// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#ifndef DRASH_ANIMATOR_H
#define DRASH_ANIMATOR_H

#include "math.h"

namespace drash
{

enum class AnimatorBehavior : unsigned int
{
    Single = 0,
    Loop = 1,
    Bounce = 2
};

template<class T>
class CAnimator
{
public:
    CAnimator() = delete;
    CAnimator(const CAnimator &_src) = delete;
    CAnimator &operator =(const CAnimator &_src) = delete;
    CAnimator &operator =(CAnimator &&_src) = delete;

    CAnimator(CAnimator &&_src);
    CAnimator(T &_value_ref);

    inline CAnimator &Set(const T &_src);
    inline CAnimator &Set(T &&_src);
    void SetTarget(const T &_target, double _time, const AnimatorBehavior &_behavior);

    inline operator T &() const;
    inline const T &GetTarget() const;
    inline bool IsTargetSet() const;

    virtual bool Step(double _dt);

private:
    T& mValue;
    T mFromValue;
    T mTargetValue;

    double mTime = 0;
    double mTimeFull = 0;
    bool mTargetSet = false;

    AnimatorBehavior mBehavior;
};

template<class T>
CAnimator<T>::CAnimator(CAnimator &&_src):
    mValue(_src.mValue)
{
}

template<class T>
CAnimator<T>::CAnimator(T &_value_ref):
    mValue(_value_ref)
{
}

template<class T>
inline CAnimator<T> &CAnimator<T>::Set(const T &_src)
{
    mValue = _src;
    mTargetSet = false;
}

template<class T>
inline CAnimator<T> &CAnimator<T>::Set(T &&_src)
{
    mValue = std::move(_src);
    mTargetSet = false;
}

template<class T>
void CAnimator<T>::SetTarget(const T &_target, double _time, const AnimatorBehavior &_behavior)
{
    mFromValue = mValue;
    mTargetValue = _target;
    mTime = 0;
    mTimeFull = _time;
    mBehavior = _behavior;
    mTargetSet = true;
}

template<class T>
inline CAnimator<T>::operator T &() const
{
    return mValue;
}

template<class T>
inline const T &CAnimator<T>::GetTarget() const
{
    return mTargetSet == true ? mTargetValue : mValue;
}

template<class T>
inline bool CAnimator<T>::IsTargetSet() const
{
    return mTargetSet;
}

template<class T>
bool CAnimator<T>::Step(double _dt)
{
    if (mTargetSet == true)
    {
        mTime += _dt;

        double k = drash::math::Min<float>(mTimeFull, mTime);

        if (drash::math::Abs(mTimeFull - k) < 0.000001)
        {
            if (mBehavior == AnimatorBehavior::Single)
            {
                mValue = mTargetValue;
                mTargetSet = false;
            }
            else if (mBehavior == AnimatorBehavior::Loop)
            {
                mValue = mFromValue;
                mTime = 0;
            }
            else if (mBehavior == AnimatorBehavior::Bounce)
            {
                mValue = mTargetValue;
                mTargetValue = mFromValue;
                mFromValue = mValue;
                mTime = 0;
            }
        }
        else
        {
            mValue = mTargetValue;
            mValue -= mFromValue;
            mValue *= k / mTimeFull;
            mValue += mFromValue;
        }

        return true;
    }

    return false;
}

} // namespace drash

#endif // DRASH_ANIMATOR_H
