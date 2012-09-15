#pragma once
#ifndef CANIMATEDPARAM_H
#define CANIMATEDPARAM_H

#include <cmath>

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
    ~CAnimatedParam();

    void Set( const T &_val );
    void SetTarget( const T &_target, double _time, const AnimationBehavior &_behavior );

    const T &Get() const;
    const T &GetTarget() const;

    bool IsTargetSet() const;

    void Step( double _dt );

private:
    T mValue;
    T mFromValue;
    T mTargetValue;
    T mDeltaValue;
    double mElaspedTime;
    double mAnimationTime;
    bool mTargetSet;
    AnimationBehavior mBehavior;
};

template <class T>
CAnimatedParam<T>::CAnimatedParam():
    mValue(),
    mFromValue(),
    mTargetValue(),
    mDeltaValue(),
    mElaspedTime(0.0),
    mAnimationTime(0.0),
    mTargetSet(false),
    mBehavior(AnimationBehaviorSingle)
{
}

template <class T>
CAnimatedParam<T>::~CAnimatedParam()
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
    mDeltaValue = mTargetValue - mFromValue;
    mAnimationTime = _time;
    mElaspedTime = 0.0;
    mBehavior = _behavior;
}

template <class T>
const T &CAnimatedParam<T>::Get() const
{
    return mValue;
}

template <class T>
const T &CAnimatedParam<T>::GetTarget() const
{
    return mTargetValue;
}

template <class T>
bool CAnimatedParam<T>::IsTargetSet() const
{
    return mTargetSet;
}

template <class T>
void CAnimatedParam<T>::Step( double _dt )
{
    if ( mTargetSet == true )
    {
        mElaspedTime += _dt;

        double k = std::min( mAnimationTime, mElaspedTime );

        if ( std::fabs(mAnimationTime - k) < 0.000001 )
        {
            if (mBehavior == AnimationBehaviorSingle)
            {
                mValue = mTargetValue;
                mTargetSet = false;
            }
            else if ( mBehavior == AnimationBehaviorLoop )
            {
                mValue = mFromValue;
                mElaspedTime = 0;
            }
            else if ( mBehavior == AnimationBehaviorBounce )
            {
                mValue = mTargetValue;
                mTargetValue = mFromValue;
                mFromValue = mValue;
                mDeltaValue = -mDeltaValue;
                mElaspedTime = 0;
            }
        }
        else
        {
            mValue = mDeltaValue;
            mValue *= k / mAnimationTime;
            mValue += mFromValue;
        }
    }
}

} // namespace drash

#endif // CANIMATEDPARAM_H
