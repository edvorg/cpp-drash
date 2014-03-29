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

#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <vector>
#include <functional>
#include <stack>

#include "misc.hpp"
#include "dimensions.hpp"

namespace drash {

    template <class PARTICLE> class ParticleSystem {

      public:
        using Period = std::function<float()>;
        using Life = std::function<float()>;
        using PostSpawn = std::function<void(PARTICLE &)>;
        using PreDestroy = std::function<void(PARTICLE &)>;

        ParticleSystem() = default;
        virtual ~ParticleSystem() = default;

        virtual void Init();
        virtual void Update(double dt);
        virtual void Draw();
        virtual void Release();
        // unuse all particles
        virtual void Clean();
        // resize system pool
        virtual void Resize(int newSize);

        // collide with some object
        template <class ANOTHER>
        void Collide(ANOTHER &another,
                     std::function<void(PARTICLE &, ANOTHER &)> callback);
        // collide with another particle system
        template <class ANOTHER>
        void Collide(ParticleSystem<ANOTHER> &another,
                     std::function<void(PARTICLE &, ANOTHER &)> callback);

        void FieldSize(float newWidth, float newHeight);
        void FieldPos(float newX, float newY);
        inline void MarginLeft(float left) { marginLeft = left; }
        inline void MarginTop(float top) { marginTop = top; }
        inline void MarginRight(float right) { marginRight = right; }
        inline void MarginBottom(float bottom) { marginBottom = bottom; }

        // push function returning spawn period (top will be used)
        void PushPeriod(Period newPeriod);
        void PopPeriod();
        Period TopPeriod();

        // push function returning life length for created particles (top will
        // be used)
        void PushLife(Life newLife);
        void PopLife();
        Life TopLife();

        // push callback for particle post spawn event (top will be used)
        void PushPostSpawn(PostSpawn newPostSpawn);
        void PopPostSpawn();
        PostSpawn TopPostSpawn();

        // push callback for particle pre destroy event (top will be used)
        void PushPreDestroy(PreDestroy newPreDestroy);
        void PopPreDestroy();
        PreDestroy TopPreDestroy();

        // spawn particles every period seconds
        void SetSpawning(bool enable);

        inline float GetFieldWidth() const { return params.fieldWidth; }
        inline float GetFieldHeight() const { return params.fieldHeight; }

      protected:
        void Spawn();
        // destroy by index ind pool
        void Destroy(unsigned int index);

      private:
        std::vector<PARTICLE> pool;
        int poolSize = 100;
        double spawnTimer = 0.0;
        double spawnPeriod = 1.0;
        // number of used particles
        int used = 0;
        RandomizeParams params;
        // spawn particles every period seconds
        bool spawning = true;

        // field left bottom corner
        float x = 0.0f;
        float y = 0.0f;

        // margins
        float marginLeft = 0.0f;
        float marginRight = 0.0f;
        float marginTop = 0.0f;
        float marginBottom = 0.0f;

        // customization functions
        // must return particles spawn period
        std::stack<Period> periods;
        // optional particle life limit
        std::stack<Life> lifes;
        // optional particle post process after creation
        std::stack<PostSpawn> postSpawns;
        // optional particle pre process after destroy
        std::stack<PreDestroy> preDestroys;
    };

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Init() {
        pool.resize(poolSize);
        for (unsigned int i = 0; i < poolSize; i++) {
            pool[i].Init();
        }
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Update(double dt) {
        // spawn new pool on field
        if (spawning) {
            spawnTimer += dt;
            if (periods.size()) {
                spawnPeriod = periods.top()();
            }
            if (spawnTimer > spawnPeriod && used < poolSize) {
                Spawn();
                spawnTimer = 0;
            }
        }

        // update active pool
        for (unsigned int i = 0; i < used; ++i) {
            pool[i].SetPos(pool[i].GetPosX() + pool[i].GetVelX() * dt,
                           pool[i].GetPosY() + pool[i].GetVelY() * dt);
            pool[i].SetAngle(pool[i].GetAngle() + pool[i].GetVelAngle() * dt);
            pool[i].SetLifeTimer(pool[i].GetLifeTimer() + dt);
            pool[i].Update(dt);

            // destroy particles which will be never seen or set dead
            // for extending field use of margins is recommended
            if (pool[i].GetDead() ||
                (lifes.size() && pool[i].GetLifeTimer() > lifes.top()()) ||
                (pool[i].GetPosY() < y && pool[i].GetVelY() < 0.0f) ||
                (pool[i].GetPosY() > y + params.fieldHeight &&
                 pool[i].GetVelY() > 0.0f) ||
                (pool[i].GetPosX() < x && pool[i].GetVelX() < 0.0f) ||
                (pool[i].GetPosX() > x + params.fieldWidth &&
                 pool[i].GetVelX() > 0.0f) ||
                pool[i].GetPosY() < y - marginBottom ||
                pool[i].GetPosY() > y + params.fieldHeight + marginTop ||
                pool[i].GetPosX() < x - marginLeft ||
                pool[i].GetPosX() > x + params.fieldWidth + marginRight) {

                pool[i].SetDead(false);
                Destroy(i);
            }
        }
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Draw() {
        for (unsigned int i = 0; i < used; ++i) {
            pool[i].Draw();
        }
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Release() {
        for (unsigned int i = 0; i < poolSize; i++) {
            pool[i].Release();
        }
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Clean() {
        used = 0;
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::Resize(int newSize) {
        pool.resize(newSize);
        poolSize = newSize;
        used = 0;
    }

    template <class PARTICLE>
    template <class ANOTHER>
    void ParticleSystem<PARTICLE>::Collide(
        ANOTHER &another, std::function<void(PARTICLE &, ANOTHER &)> callback) {
        // TODO optimize this. complexity n now, use aabb or something else
        for (unsigned int i = 0; i < used; ++i) {
            if (another.GetDimensions().Intersect(pool[i].GetDimensions())) {
                callback(pool[i], another);
            }
        }
    }

    template <class PARTICLE>
    template <class ANOTHER>
    void ParticleSystem<PARTICLE>::Collide(
        ParticleSystem<ANOTHER> &another,
        std::function<void(PARTICLE &, ANOTHER &)> callback) {
        // TODO optimize this. complexity n^2 now, use aabb or something else
        auto reversed = [&](ANOTHER &a, PARTICLE &p) { callback(p, a); };
        for (unsigned i = 0; i < used; i++) {
            another.template Collide<PARTICLE>(pool[i], reversed);
        }
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::FieldSize(float newWidth, float newHeight) {
        params.fieldWidth = newWidth;
        params.fieldHeight = newHeight;
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::FieldPos(float newX, float newY) {
        x = newX;
        y = newY;
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::PushPeriod(Period newPeriod) {
        periods.push(newPeriod);
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::PopPeriod() {
        if (periods.size()) {
            periods.pop();
        }
    }

    template <class PARTICLE>
    typename ParticleSystem<PARTICLE>::Period
    ParticleSystem<PARTICLE>::TopPeriod() {
        return periods.top();
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::PushLife(Life newLife) {
        lifes.push(newLife);
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::PopLife() {
        if (lifes.size()) {
            lifes.pop();
        }
    }

    template <class PARTICLE>
    typename ParticleSystem<PARTICLE>::Life
    ParticleSystem<PARTICLE>::TopLife() {
        return lifes.top();
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::PushPostSpawn(PostSpawn newPostSpawn) {
        postSpawns.push(newPostSpawn);
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::PopPostSpawn() {
        if (postSpawns.size()) {
            postSpawns.pop();
        }
    }

    template <class PARTICLE>
    typename ParticleSystem<PARTICLE>::PostSpawn
    ParticleSystem<PARTICLE>::TopPostSpawn() {
        return postSpawns.top();
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::PushPreDestroy(PreDestroy newPreDestroy) {
        preDestroys.push(newPreDestroy);
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::PopPreDestroy() {
        if (preDestroys.size()) {
            preDestroys.pop();
        }
    }

    template <class PARTICLE>
    typename ParticleSystem<PARTICLE>::PreDestroy
    ParticleSystem<PARTICLE>::TopPreDestroy() {
        return preDestroys.top();
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::SetSpawning(bool enable) {
        spawning = enable;
    }

    template <class PARTICLE> void ParticleSystem<PARTICLE>::Spawn() {
        if (used < poolSize) {
            auto index = used;
            used++;
            pool[index].SetLifeTimer(0.0f);
            if (postSpawns.size()) {
                postSpawns.top()(pool[index]);
            }
        }
    }

    template <class PARTICLE>
    void ParticleSystem<PARTICLE>::Destroy(unsigned int index) {
        if (index < used) {
            if (preDestroys.size()) {
                preDestroys.top()(pool[index]);
            }
            pool[index].SetLifeTimer(0.0f);
            used--;
            if (index < used) {
                pool[index] = pool[used];
            }
        }
    }
}

#endif // PARTICLESYSTEM_HPP
