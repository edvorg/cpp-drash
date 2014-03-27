#include "screenshaker.hpp"

#include <cmath>

namespace drash {
    
    void ScreenShaker::Shake() {
        shake = true;
    }

    void ScreenShaker::Step(double dt) {
        if (shake) {
            shakeTimer += dt;
            if (shakeTimer > shakeTime) {
                shake = false;
                translate = 0.0f;
                shakeTimer = 0.0f;
            }
            else {
                auto amplitude = (1.0 - shakeTimer / shakeTime) * 5.0;
                auto angle = shakeTimer / shakeTime * M_PI * 2.0f * 10.0f;
                translate = amplitude * sin(angle);
            }
        }
    }

}
