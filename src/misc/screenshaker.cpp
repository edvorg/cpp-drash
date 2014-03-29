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

#include "screenshaker.hpp"

#include <cmath>

namespace drash {

    void ScreenShaker::Shake() { shake = true; }

    void ScreenShaker::Step(double dt) {
        if (shake) {
            shakeTimer += dt;
            if (shakeTimer > shakeTime) {
                shake = false;
                translate = 0.0f;
                shakeTimer = 0.0f;
            } else {
                auto amplitude = (1.0 - shakeTimer / shakeTime) * 5.0;
                auto angle = shakeTimer / shakeTime * M_PI * 2.0f * 10.0f;
                translate = amplitude * sin(angle);
            }
        }
    }
}