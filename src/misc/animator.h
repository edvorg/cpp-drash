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

#ifndef DRASH_ANIMATOR_H
#define DRASH_ANIMATOR_H

#include <utility>
#include "math.h"

namespace drash {

    enum class AnimatorBehavior : unsigned int {
        Single = 0,
        Loop = 1,
        Bounce = 2
    };

    template <class T> class CAnimator {
      public:
        CAnimator() = delete;
        CAnimator(const CAnimator& _src) = delete;
        CAnimator& operator=(const CAnimator& _src) = delete;
        CAnimator& operator=(CAnimator&& _src) = delete;

        CAnimator(CAnimator&& _src);
        CAnimator(T& _value_ref);

        CAnimator& operator=(const T& _src);
        CAnimator& operator=(T&& _src);
        CAnimator& Set(const T& _src);
        CAnimator& Set(T&& _src);
        void SetTarget(const T& _target, double _time,
                       const AnimatorBehavior& _behavior);
        inline void RemoveTarget();

        inline operator const T&() const;
        inline const T& Get() const;
        inline const T& GetTarget() const;
        inline double GetTime() const;
        inline double GetTimeRemains() const;
        inline bool IsTargetSet() const;

        bool Step(double _dt);

      private:
        T& value;
        T fromValue;
        T targetValue;

        double time = 0;
        double timeFull = 0;
        bool targetSet = false;

        bool valueUpdated = false;

        AnimatorBehavior behavior;
    };

    template <class T>
    CAnimator<T>::CAnimator(CAnimator&& _src)
        : value(_src.value) {}

    template <class T>
    CAnimator<T>::CAnimator(T& _value_ref)
        : value(_value_ref) {}

    template <class T> CAnimator<T>& CAnimator<T>::operator=(const T& _src) {
        value = _src;
        targetSet = false;
        valueUpdated = true;

        return *this;
    }

    template <class T> CAnimator<T>& CAnimator<T>::operator=(T&& _src) {
        value = std::move(_src);
        targetSet = false;
        valueUpdated = true;

        return *this;
    }

    template <class T> CAnimator<T>& CAnimator<T>::Set(const T& _src) {
        value = _src;
        targetSet = false;
        valueUpdated = true;

        return *this;
    }

    template <class T> CAnimator<T>& CAnimator<T>::Set(T&& _src) {
        value = std::move(_src);
        targetSet = false;
        valueUpdated = true;

        return *this;
    }

    template <class T>
    void CAnimator<T>::SetTarget(const T& _target, double _time,
                                 const AnimatorBehavior& _behavior) {
        fromValue = value;
        targetValue = _target;
        time = 0;
        timeFull = _time;
        behavior = _behavior;
        targetSet = true;
    }

    template <class T> inline void CAnimator<T>::RemoveTarget() {
        targetSet = false;
    }

    template <class T> inline CAnimator<T>::operator const T&() const {
        return value;
    }

    template <class T> inline const T& CAnimator<T>::Get() const {
        return value;
    }

    template <class T> inline const T& CAnimator<T>::GetTarget() const {
        if (targetSet == true) {
            return targetValue;
        } else {
            return value;
        }
    }

    template <class T> inline double CAnimator<T>::GetTime() const {
        return time;
    }

    template <class T> inline double CAnimator<T>::GetTimeRemains() const {
        return math::Clamp<double>(timeFull - time, 0, timeFull);
    }

    template <class T> inline bool CAnimator<T>::IsTargetSet() const {
        return targetSet;
    }

    template <class T> bool CAnimator<T>::Step(double _dt) {
        if (targetSet == true) {
            time += _dt;

            double k = drash::math::Min<float>(timeFull, time);

            if (drash::math::Abs(timeFull - k) < 0.000001) {
                if (behavior == AnimatorBehavior::Single) {
                    value = targetValue;
                    targetSet = false;
                } else if (behavior == AnimatorBehavior::Loop) {
                    value = fromValue;
                    time = 0;
                } else if (behavior == AnimatorBehavior::Bounce) {
                    value = targetValue;
                    targetValue = fromValue;
                    fromValue = value;
                    time = 0;
                }
            } else {
                value = targetValue;
                value -= fromValue;
                value *= k / timeFull;
                value += fromValue;
            }

            return true;
        } else if (valueUpdated == true) {
            valueUpdated = false;
            return true;
        } else {
            return false;
        }
    }

} // namespace drash

#endif // DRASH_ANIMATOR_H
