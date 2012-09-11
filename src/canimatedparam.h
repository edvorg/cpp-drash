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
    double GetAnimationTime() const;

    void Step( double _dt );

private:
    T mValue;
    T mFromValue;
    T mTargetValue;
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
void CAnimatedParam<T>::SetTarget(const T &_target , double _time , const AnimationBehavior &_behavior)
{
    mTargetSet = true;
    mFromValue = mValue;
    mTargetValue = _target;
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
double CAnimatedParam<T>::GetAnimationTime() const
{
    return mAnimationTime;
}

template <class T>
void CAnimatedParam<T>::Step( double _dt )
{
    if ( mTargetSet == true )
    {
        mElaspedTime += _dt;

        double k = std::min( mAnimationTime, mElaspedTime ) / mAnimationTime;

        if ( std::fabs(1.0 - k) < 0.000001 )
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
                mElaspedTime = 0;
            }
        }
        else
        {
            mValue = k * mTargetValue + ( 1.0 - k ) * mFromValue;
        }
    }
}

} // namespace drash

#endif // CANIMATEDPARAM_H
