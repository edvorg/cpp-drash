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

#endif // SCREENSHAKER_HPP
