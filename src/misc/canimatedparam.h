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

#pragma once
#ifndef CANIMATEDPARAM_H
#define CANIMATEDPARAM_H

namespace drash
{

enum AnimationBehavior
{
    AnimationBehaviorLoop,
    AnimationBehaviorSingle,
    AnimationBehaviorBounce
};

template <class T>
class CAnimatedParam
{
public:
    CAnimatedParam();

    void Set( const T &_val );
    void SetTarget( const T &_target, double _time, const AnimationBehavior &_behavior );

    inline const T &Get() const;
    inline const T &GetTarget() const;
    inline double GetTimeRemains() const;

    inline bool IsTargetSet() const;

    void Step( double _dt );

private:
    T mValue;
    T mFromValue;
    T mTargetValue;
    double mTimeElapsed;
    double mTimeFull;
    bool mTargetSet;
    AnimationBehavior mBehavior;
};

template <class T>
CAnimatedParam<T>::CAnimatedParam():
    mValue(),
    mFromValue(),
    mTargetValue(),
    mTimeElapsed(0.0),
    mTimeFull(0.0),
    mTargetSet(false),
    mBehavior(AnimationBehaviorSingle)
{
}

template <class T>
void CAnimatedParam<T>::Set( const T &_val )
{
    mTargetSet = false;
    mValue = _val;
}

template <class T>
void CAnimatedParam<T>::SetTarget( const T &_target , double _time , const AnimationBehavior &_behavior )
{
    mTargetSet = true;
    mFromValue = mValue;
    mTargetValue = _target;
    mTimeFull = _time;
    mTimeElapsed = 0.0;
    mBehavior = _behavior;
}

template <class T>
inline const T &CAnimatedParam<T>::Get() const
{
    return mValue;
}

template <class T>
inline const T &CAnimatedParam<T>::GetTarget() const
{
    return mTargetValue;
}

template <class T>
inline double CAnimatedParam<T>::GetTimeRemains() const
{
    return mTimeFull - mTimeElapsed;
}

template <class T>
inline bool CAnimatedParam<T>::IsTargetSet() const
{
    return mTargetSet;
}

template <class T>
void CAnimatedParam<T>::Step( double _dt )
{
    if ( mTargetSet == true )
    {
        mTimeElapsed += _dt;

        double k = drash::math::Min( mTimeFull, mTimeElapsed );

        if ( drash::math::Abs( mTimeFull - k ) < 0.000001 )
        {
            if (mBehavior == AnimationBehaviorSingle)
            {
                mValue = mTargetValue;
                mTargetSet = false;
            }
            else if ( mBehavior == AnimationBehaviorLoop )
            {
                mValue = mFromValue;
                mTimeElapsed = 0;
            }
            else if ( mBehavior == AnimationBehaviorBounce )
            {
                mValue = mTargetValue;
                mTargetValue = mFromValue;
                mFromValue = mValue;
                mTimeElapsed = 0;
            }
        }
        else
        {
            mValue = mTargetValue;
            mValue -= mFromValue;
            mValue *= k / mTimeFull;
            mValue += mFromValue;
        }
    }
}

} // namespace drash

#endif // CANIMATEDPARAM_H
