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

#ifndef DRASH_VARANIMATOR_H
#define DRASH_VARANIMATOR_H

#include <utility>
#include "math.h"
#include <react/react.hpp>

namespace drash {

    using react::Var;

    enum class VarAnimatorBehavior : unsigned int {
        Single = 0,
        Loop = 1,
        Bounce = 2
    };

    template <class T> class VarAnimator;
    
    template <class T> class VarAnimator<Var<T>> {
    public:
        VarAnimator() = delete;
        VarAnimator& operator=(const VarAnimator& _src) = delete;

        VarAnimator(Var<T>& _value_ref) : value(_value_ref) {}

        VarAnimator& operator=(const T& _src) {
            Set(_src);
            return *this;
        }
        
        template<class U>
        VarAnimator& operator=(U&& _src) {
            value = std::forward<U>(_src);
            targetSet = false;
            valueUpdated = true;

            return *this;
        }
        
        template<class U>
        VarAnimator& Set(const U& _src) {
            value = _src;
            targetSet = false;
            valueUpdated = true;

            return *this;
        }
        
        template<class U>
        VarAnimator& Set(U&& _src) {
            value = std::forward<U>(_src);
            targetSet = false;
            valueUpdated = true;

            return *this;
        }
        
        void SetTarget(const T& _target, double _time,
                       const VarAnimatorBehavior& _behavior){
            fromValue = value;
            targetValue = _target;
            time = 0;
            timeFull = _time;
            behavior = _behavior;
            targetSet = true;
        }

        void RemoveTarget() {
            targetSet = false;
        }

        operator const T&() const {
            return value();
        }

        const T& Get() const {
            return value();
        }

        const T& GetTarget() const {
            if (targetSet == true) {
                return targetValue;
            } else {
                return value();
            }
        }

        double GetTime() const {
            return time;
        }

        double GetTimeRemains() const {
            return math::Clamp<double>(timeFull - time, 0, timeFull);
        }

        bool IsTargetSet() const {
            return targetSet;
        }

        bool Step(double _dt) {
            if (targetSet == true) {
                time += _dt;

                double k = drash::math::Min<float>(timeFull, time);

                if (drash::math::Abs(timeFull - k) < 0.000001) {
                    if (behavior == VarAnimatorBehavior::Single) {
                        value = targetValue;
                        targetSet = false;
                    } else if (behavior == VarAnimatorBehavior::Loop) {
                        value = fromValue;
                        time = 0;
                    } else if (behavior == VarAnimatorBehavior::Bounce) {
                        value = targetValue;
                        targetValue = fromValue;
                        fromValue = value;
                        time = 0;
                    }
                } else {
                    auto tmp = targetValue;
                    tmp -= fromValue;
                    tmp *= k / timeFull;
                    tmp += fromValue;
                    value = tmp;
                }

                return true;
            } else if (valueUpdated == true) {
                valueUpdated = false;
                return true;
            } else {
                return false;
            }
        }

    private:
        Var<T>& value;
        T fromValue;
        T targetValue;

        double time = 0;
        double timeFull = 0;
        bool targetSet = false;

        bool valueUpdated = false;

        VarAnimatorBehavior behavior;
    };

} // namespace drash

#endif // DRASH_VARANIMATOR_H
