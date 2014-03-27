#ifndef SCREENSHAKER_HPP
#define SCREENSHAKER_HPP

namespace drash {

    // screen shake effect used in by e.g. explosions
    class ScreenShaker {
    public:
        // starts screen shake effect
        void Shake();

        void Step(double dt);
        // apply transformation matrix of shake effect
        float GetTranslate() const { return translate; }

    protected:
    private:
        const float shakeTime = 1.0f;
        float shakeTimer = 0.0f;
        bool shake = false;
        float translate = 0.0f;
    };

}

#endif// SCREENSHAKER_HPP















