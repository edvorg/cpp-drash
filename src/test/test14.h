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

#pragma once
#ifndef DRASH_TEST_TEST14_H
#define DRASH_TEST_TEST14_H

#include "../app/app.h"
#include "../greng/pointlight.h"
#include "../greng/spotlight.h"

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace drash {

    class Player;
    class Figure;

    namespace test {

        /// "Powerless" — single-level destructible-platformer scenario.
        /// See docs/powerless.md for the design rationale.
        class Test14 : public App {
        public:
            Test14(greng::Greng& greng);
            ~Test14() = default;

        private:
            enum class State {
                Intro,
                HasGun,
                Chamber2,
                Chamber3,
                Victory,
            };

            virtual void Step(double _dt) override;
            virtual void Render() override;

            // Setup
            void InitCamera();
            void InitTemplates();
            void InitWorld();
            void InitLamps();
            void InitInteractive();
            void InitPlayer();
            void InitProcessors();

            // Step helpers
            void UpdateCameraFollow();
            void UpdateLamps();
            void UpdateProjectiles(double _dt);
            void UpdateHazards();
            void UpdateCaptions(double _dt);
            void UpdateProgressNarration();
            void UpdateStateTransitions();

            // Render helpers
            void DrawCaptions();
            void DrawGlowingMarkers();

            // Game state
            void EnterState(State _s);
            void Narrate(const std::string& _text, double _duration);
            void SpawnProjectile();

            // Construction primitives
            SceneObject* CreateBox(const std::string& _name, float _halfW,
                                   float _halfH, const Vec3f& _pos, bool _dyn,
                                   bool _fixedRot = true);
            SceneObject* CreateLamp(const std::string& _geoName,
                                    SceneObject* _ceilingAnchor, float _xMid,
                                    float _ceilingY, float _ropeLen);
            SceneObject* CreateDestructibleDoor(float _x, float _yBottom,
                                                int _bricks);

            // Scene
            greng::Camera* camera = nullptr;
            greng::Camera* uiCamera = nullptr;
            greng::PointLight ambientPoint;
            greng::SpotLight  lamp1Spot;
            greng::SpotLight  lamp2Spot;
            greng::SpotLight  lamp3Spot;

            // Player
            Player* player = nullptr;
            SceneObject* playerObj = nullptr;
            bool playerHasGun = false;

            // Interactive
            SceneObject* gunPickup = nullptr;
            SceneObject* button = nullptr;
            SceneObject* chamber2Trigger = nullptr;
            SceneObject* chamber3Trigger = nullptr;
            SceneObject* climbTrigger = nullptr;

            // Lamps & projectiles. Anchor positions are stored alongside
            // the bulbs so we can draw the rope as a line every frame.
            std::vector<SceneObject*> ambientLamps;
            std::vector<Vec3f>        ambientLampAnchors;
            std::vector<SceneObject*> hazardLamps;
            std::vector<Vec3f>        hazardLampAnchors;
            std::map<SceneObject*, double> projectileLife;

            // State
            State state = State::Intro;
            double stateTime = 0.0;
            double totalTime = 0.0;

            // Camera follow
            float cameraTargetX = -25.0f;
            float cameraDeadband = 2.5f;

            // Jump: PlayerMessage::Jump applies a 0.5 impulse which is
            // visually invisible, so we apply our own impulse and gate it
            // on ground contact + a small cooldown.
            int playerGroundContacts = 0;
            double lastJumpTime = -100.0;

            // Red emergency-light blink factor in [0..1], updated each
            // frame from totalTime. 0 = dim red, 1 = bright red.
            float redBlink = 1.0f;

            // Auto-quit after Victory so the user gets clear closure
            // even if they miss the "press ctrl q to exit" caption.
            // Set in EnterState(Victory). Sentinel = no quit pending.
            double autoQuitAt = -1.0;

            // Hazard respawn
            Vec3f chamber3Spawn = Vec3f(20.0f, 4.0f, 0.0f);

            // Caption queue (FIFO; one shown at a time)
            struct Caption {
                std::string text;
                double remaining;
                double total;
            };
            std::deque<Caption> captions;

            // Progress narration: each milestone fires once
            bool said_door1Hit = false;
            bool said_door1Half = false;
            bool said_climbHigh = false;
            bool said_lamp2Warn = false;
            bool said_lamp3Warn = false;
            bool said_buttonNear = false;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST14_H
