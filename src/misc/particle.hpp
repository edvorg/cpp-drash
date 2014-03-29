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

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

namespace drash {

    class Particle {

    public:
        inline void SetPosX(float newX) { x = newX; }
        inline void SetPosY(float newY) { y = newY; }
        inline void SetPos(float newX, float newY) {
            x = newX;
            y = newY;
        }

        inline void IncPos(float incX, float incY) {
            x += incX;
            y += incY;
        }
        inline void DecPos(float decX, float decY) {
            x -= decX;
            y -= decY;
        }

        inline void IncPosX(float incX) { x += incX; }
        inline void DecPosX(float decX) { x -= decX; }

        inline void IncPosY(float incY) { y += incY; }
        inline void DecPosY(float decY) { y -= decY; }

        inline void SetVelX(float newVelX) { velX = newVelX; }
        inline void SetVelY(float newVelY) { velY = newVelY; }
        inline void SetVel(float newVelX, float newVelY) {
            velX = newVelX, velY = newVelY;
        }

        inline void IncVel(float incVelX, float incVelY) {
            velX += incVelX;
            velY += incVelY;
        }
        inline void DecVel(float decVelX, float decVelY) {
            velX -= decVelX;
            velY -= decVelY;
        }

        inline void IncVelX(float incVelX) { velX += incVelX; }
        inline void DecVelX(float decVelX) { velX -= decVelX; }

        inline void IncVelY(float incVelY) { velY += incVelY; }
        inline void DecVelY(float decVelY) { velY -= decVelY; }

        inline void SetAngle(float newAngle) { angle = newAngle; }
        inline void SetVelAngle(float newVelAngle) { velAngle = newVelAngle; }

        inline void SetSize(float newSize) { size = newSize; }
        inline void SetDead(bool newDead) { dead = newDead; }
        inline void SetLifeTimer(float newLifeTimer) {
            lifeTimer = newLifeTimer;
        }

        inline float GetPosX() const { return x; }
        inline float GetPosY() const { return y; }

        inline float GetVelX() const { return velX; }
        inline float GetVelY() const { return velY; }

        inline float GetAngle() const { return angle; }
        inline float GetVelAngle() const { return velAngle; }

        inline float GetSize() const { return size; }
        inline bool GetDead() const { return dead; }
        inline float GetLifeTimer() const { return lifeTimer; }

    protected:
    private:
        float x = 0.0f;
        float y = 0.0f;
        float angle = 0;
        float velX = 0;
        float velY = 0;
        float velAngle = 0;
        float size = 1.0f;
        bool dead = false;
        float lifeTimer = 0.0f;
    };
}

#endif // PARTICLE_HPP
