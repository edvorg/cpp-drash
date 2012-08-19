#ifndef CANIMATEDPARAM_H
#define CANIMATEDPARAM_H

#include <cmath>

namespace drash
{

template <class T>
class CAnimatedParam
{
public:
    CAnimatedParam();
    ~CAnimatedParam();

    void Set( const T &_val );
    void SetTarget( const T &_target, double _time );

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
};

template <class T>
CAnimatedParam<T>::CAnimatedParam():
    mValue(),
    mFromValue(),
    mTargetValue(),
    mElaspedTime(0.0),
    mAnimationTime(0.0),
    mTargetSet(false)
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
void CAnimatedParam<T>::SetTarget( const T &_target , double _time )
{
    mTargetSet = true;
    mFromValue = mValue;
    mTargetValue = _target;
    mAnimationTime = _time;
    mElaspedTime = 0.0;
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

        double k = ( mAnimationTime < mElaspedTime ? mAnimationTime : mElaspedTime ) / mAnimationTime;

        mValue = k * mTargetValue + ( 1.0 - k ) * mFromValue;
    }
    else
    {
    }
}

} // namespace drash

#endif // CANIMATEDPARAM_H
