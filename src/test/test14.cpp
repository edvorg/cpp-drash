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

#include "test14.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>

#include "../app/appeventprocessor.h"
#include "../greng/camera.h"
#include "../greng/cameramanager.h"
#include "../greng/renderer.h"
#include "../misc/plane.h"
#include "../players/player.h"
#include "../players/playerssystem.h"
#include "../scene/figure.h"
#include "../scene/geometrymanager.h"
#include "../scene/scene.h"
#include "../scene/sceneobject.h"

#include <cmath>
#include <sstream>
#include <string>

namespace drash {
namespace test {

namespace {
    // Level layout (X is horizontal, Y is up). One continuous corridor
    // with three destructible doors gating four logical chambers.
    constexpr float kFloorY = 0.0f;
    constexpr float kCeilingY = 18.0f;
    constexpr float kChamber1Left = -32.0f;
    constexpr float kChamber1Right = -10.0f;
    constexpr float kChamber2Right = 18.0f;
    constexpr float kChamber3Right = 50.0f;
    constexpr float kChamber4Right = 66.0f;
    // Where door 2's bottom brick rests — at the chamber-2 landing.
    // The lower static wall fills 0..kDoor2BottomY so the floor route
    // is blocked and the climb is mandatory.
    constexpr float kDoor2BottomY = 11.5f;

    constexpr float kPlayerSpawnX = -27.0f;
    constexpr float kGunPickupX = -16.0f;
    constexpr float kButtonX = 60.0f;

    // Bitmap-font caption layout in the ortho ui camera (size 200x200ish).
    // Renderer::DrawChar only supports a-z, 0-9 and '_' as line-strip
    // glyphs. Other characters render as nothing, so all caption text
    // must be sanitized to that alphabet.
    constexpr float kUiOrthoHalf = 100.0f;
    constexpr float kCaptionCharW = 2.0f;
    constexpr float kCaptionCharH = 4.0f;
    constexpr float kCaptionMarginBottom = 10.0f;

    // Mirror Renderer::CharWidth so caption centring matches the actual
    // glyph advance.
    inline float captionCharAdvance(char c) {
        if (c == 'l' || c == '1' || c == 'i') {
            return 0.5f;
        }
        if (c == 'f' || c == 't' || c == 'k' || c == 'j' || c == 'n'
            || c == 'r' || c == 'v' || c == 'x') {
            return 1.5f;
        }
        return 2.5f;
    }
}

// ============================================================================
// Constructor
// ============================================================================

Test14::Test14(greng::Greng& greng) : App(greng) {
    InitCamera();
    InitTemplates();
    InitWorld();
    InitLamps();
    InitInteractive();
    InitPlayer();
    InitProcessors();

    GetDebugRenderer().SetTexCoordsScale(0.2f);

    // Opening narration. The user is brand new — over-explain.
    // The bitmap font only supports a-z, 0-9 and underscore, so all
    // caption text is lowercase ASCII letters and digits only.
    Narrate("power is out", 3.5);
    Narrate("you wake up in the dark", 3.5);
    Narrate("get up and find a way out", 4.0);
    Narrate("move with a and d keys", 4.0);
    Narrate("jump with space", 4.0);
    Narrate("something glows on the floor ahead", 4.5);
}

// ============================================================================
// Setup
// ============================================================================

void Test14::InitCamera() {
    greng::CameraParams cp;
    cp.pos.Set(kPlayerSpawnX, 6, 38);
    cp.rotation.x = -static_cast<float>(M_PI) / 28.0f;
    camera = GetGreng().GetCameraManager().CreateCamera(cp);
    GetDebugRenderer().SetCamera(camera);
    cameraTargetX = kPlayerSpawnX;

    // Ortho camera for HUD captions. Range is roughly -100..100 horizontally,
    // and -100/aspect..100/aspect vertically.
    uiCamera = GetGreng().GetCameraManager().CreateCamera({});
    uiCamera->GetOrtho() = true;
    uiCamera->GetOrthoSize() =
        { kUiOrthoHalf,
          kUiOrthoHalf / GetGreng().GetViewport().GetAspectRatio() };
    uiCamera->GetPos() = { 0, 0, 10 };
    uiCamera->GetDepthOfView() = 100;
}

void Test14::InitTemplates() {
    // Pull templates.txt and bake destruction graphs so the destructible
    // doors can fragment when shot. test6 does the same dance.
    GetGeometryManager().Load();
    auto& geos = GetGeometryManager().GetGeometries();
    for (auto& kv : geos) {
        if (kv.second != nullptr) {
            kv.second->ComputeDestructionGraph(0.5f);
        }
    }

    // Test14 uses its own dual-light shader (shader14) so the player
    // can carry a torch through a chamber lit by emergency lamps. The
    // default DebugRenderer shader (shader7) is the original
    // single-light no-falloff shader that other tests rely on.
    auto* vs = GetGreng().GetVertexShaderManager().CreateShaderFromFile(
        "shaders/shader14.120.vs");
    auto* fs = GetGreng().GetFragmentShaderManager().CreateShaderFromFile(
        "shaders/shader14.120.fs");
    if (vs != nullptr && fs != nullptr) {
        auto* prog = GetGreng().GetShaderProgramManager().CreateProgram(vs, fs);
        if (prog != nullptr) {
            GetDebugRenderer().SetShaderProgram(prog);
        }
    }
}

SceneObject* Test14::CreateBox(const std::string& _name, float _halfW,
                                float _halfH, const Vec3f& _pos, bool _dyn,
                                bool _fixedRot) {
    auto* g = GetGeometryManager().CreateGeometry(_name);
    if (g == nullptr) {
        return nullptr;
    }
    g->figures.resize(1);
    g->figures[0].depth = 1.0f;
    g->figures[0].mass = 1.0f;
    g->figures[0].vertices.clear();
    g->figures[0].vertices.push_back(Vec2f(-_halfW,  _halfH));
    g->figures[0].vertices.push_back(Vec2f( _halfW,  _halfH));
    g->figures[0].vertices.push_back(Vec2f( _halfW, -_halfH));
    g->figures[0].vertices.push_back(Vec2f(-_halfW, -_halfH));

    SceneObjectParams p;
    p.pos = _pos;
    p.dynamic = _dyn;
    p.fixedRotation = _fixedRot;
    return GetGeometryManager().CreateSceneObject(_name, p);
}

void Test14::InitWorld() {
    const float spanX = kChamber4Right - kChamber1Left;
    const float midX = (kChamber1Left + kChamber4Right) * 0.5f;

    // Continuous floor and ceiling spanning the whole corridor.
    CreateBox("pl_floor", spanX * 0.5f + 1.0f, 0.6f,
              Vec3f(midX, kFloorY - 0.6f, 0), false);
    CreateBox("pl_ceiling", spanX * 0.5f + 1.0f, 0.6f,
              Vec3f(midX, kCeilingY + 0.6f, 0), false);

    // End walls.
    CreateBox("pl_wall_left", 0.6f, kCeilingY * 0.5f + 1.0f,
              Vec3f(kChamber1Left - 0.6f, kCeilingY * 0.5f, 0), false);
    CreateBox("pl_wall_right", 0.6f, kCeilingY * 0.5f + 1.0f,
              Vec3f(kChamber4Right + 0.6f, kCeilingY * 0.5f, 0), false);

    // Chamber 2 climbing platforms — three stepping stones rising up to
    // a landing right in front of door 2.
    CreateBox("pl_step1", 1.6f, 0.3f, Vec3f(-3.0f, 4.0f, 0), false);
    CreateBox("pl_step2", 1.6f, 0.3f, Vec3f( 3.0f, 7.5f, 0), false);
    CreateBox("pl_step3", 1.6f, 0.3f, Vec3f(10.0f, 10.5f, 0), false);
    CreateBox("pl_landing", 3.5f, 0.3f, Vec3f(15.0f, 11.0f, 0), false);

    // Static wall blocking the chamber-2 floor route into chamber 3.
    // Spans y=0..kDoor2BottomY at the chamber boundary, indestructible,
    // so the player MUST climb the stepping stones to reach door 2.
    CreateBox("pl_lower_wall_2", 0.7f, kDoor2BottomY * 0.5f,
              Vec3f(kChamber2Right, kDoor2BottomY * 0.5f, 0), false);

    // Three destructible doors. Door 2 is elevated to landing height
    // so the climb is required to reach it.
    CreateDestructibleDoor(kChamber1Right, 0.0f, 6);
    CreateDestructibleDoor(kChamber2Right, kDoor2BottomY, 4);
    CreateDestructibleDoor(kChamber3Right, 0.0f, 6);
}

SceneObject* Test14::CreateDestructibleDoor(float _x, float _yBottom,
                                             int _bricks) {
    // The engine's destruction trigger (scene.cpp:469) only fires when
    // the hit object is dynamic AND has more than one figure AND has
    // lived > 0.1 s. So a destructible wall must be a *dynamic*
    // multi-figure body. Build it as a vertical stack of small bricks
    // whose adjacent edges share vertices, so ComputeDestructionGraph
    // (0.5) connects them into a single body. fixedRotation + gravity
    // + whatever surface is below keeps the intact stack from tipping
    // or sliding sideways.
    static int doorCounter = 0;
    std::ostringstream nm;
    nm << "pl_door_" << doorCounter++;
    auto* g = GetGeometryManager().CreateGeometry(nm.str());
    if (g == nullptr) {
        return nullptr;
    }

    const int kBricks = _bricks;
    constexpr float kBrickHalfW = 0.7f;
    constexpr float kBrickHalfH = 0.7f;
    constexpr float kBrickH = kBrickHalfH * 2.0f;

    g->figures.resize(kBricks);
    for (int i = 0; i < kBricks; ++i) {
        FigureParams& f = g->figures[i];
        const float yc = (static_cast<float>(i)
                          - (kBricks - 1) * 0.5f) * kBrickH;
        f.depth = 1.0f;
        // Heavy enough that the player's walk impulse can't budge it.
        // Projectiles still destroy it because the destruction trigger
        // is velocity-based, not mass-based (scene.cpp:468 — speed > 10).
        f.mass = 50.0f;
        f.friction = 0.9f;
        f.restitution = 0.0f;
        f.vertices.clear();
        f.vertices.push_back(Vec2f(-kBrickHalfW, yc + kBrickHalfH));
        f.vertices.push_back(Vec2f( kBrickHalfW, yc + kBrickHalfH));
        f.vertices.push_back(Vec2f( kBrickHalfW, yc - kBrickHalfH));
        f.vertices.push_back(Vec2f(-kBrickHalfW, yc - kBrickHalfH));
    }
    g->ComputeDestructionGraph(0.5f);

    SceneObjectParams p;
    // Place the centroid so the bottom brick rests at _yBottom.
    p.pos.Set(_x, _yBottom + kBricks * kBrickH * 0.5f, 0);
    p.dynamic = true;
    p.fixedRotation = true;
    return GetGeometryManager().CreateSceneObject(nm.str(), p);
}

// ----------------------------------------------------------------------------
// Lamps
// ----------------------------------------------------------------------------

void Test14::InitLamps() {
    // Light bulb geometry — small dynamic box. Cribbed from test10:128-134.
    {
        auto* g = GetGeometryManager().CreateGeometry("pl_lamp_light");
        if (g != nullptr) {
            g->figures.resize(1);
            g->figures[0].depth = 0.4f;
            g->figures[0].mass = 0.1f;
            g->figures[0].vertices.push_back(Vec2f(-0.30f,  0.45f));
            g->figures[0].vertices.push_back(Vec2f( 0.30f,  0.45f));
            g->figures[0].vertices.push_back(Vec2f( 0.30f, -0.45f));
            g->figures[0].vertices.push_back(Vec2f(-0.30f, -0.45f));
        }
    }

    // Heavy pendulum lamp for the chamber 3 hazard — bigger and much
    // heavier so contact actually launches the player.
    {
        auto* g = GetGeometryManager().CreateGeometry("pl_lamp_heavy");
        if (g != nullptr) {
            g->figures.resize(1);
            g->figures[0].depth = 0.6f;
            g->figures[0].mass = 6.0f;
            g->figures[0].vertices.push_back(Vec2f(-0.6f,  0.9f));
            g->figures[0].vertices.push_back(Vec2f( 0.6f,  0.9f));
            g->figures[0].vertices.push_back(Vec2f( 0.6f, -0.9f));
            g->figures[0].vertices.push_back(Vec2f(-0.6f, -0.9f));
        }
    }

    // Helper: build a static ceiling anchor + dynamic bulb + rope.
    // Returns a pair so callers can also store the anchor position for
    // rope rendering.
    auto buildLamp = [&](const std::string& bulbGeo, float x, float ropeLen)
        -> std::pair<SceneObject*, Vec3f> {
        Vec3f anchorPos(x, kCeilingY - 0.4f, 0);
        std::ostringstream anchorName;
        anchorName << "pl_anchor_" << x;
        SceneObject* anchor =
            CreateBox(anchorName.str(), 0.25f, 0.25f, anchorPos, false);

        SceneObjectParams bp;
        bp.pos.Set(x + 0.001f, anchorPos.y - ropeLen, 0);
        bp.dynamic = true;
        bp.fixedRotation = false;
        SceneObject* bulb =
            GetGeometryManager().CreateSceneObject(bulbGeo, bp);
        if (anchor != nullptr && bulb != nullptr) {
            Vec3f a1(0, 0.45f, 0);
            Vec3f a2(0, 0, 0);
            GetScene().CreateJointRope(bulb, anchor, a1, a2, ropeLen);
        }
        return std::make_pair(bulb, anchorPos);
    };

    // Chamber 2 — atmospheric lamp, swings gently.
    {
        auto pair = buildLamp("pl_lamp_light", -1.0f, 6.0f);
        if (pair.first != nullptr) {
            ambientLamps.push_back(pair.first);
            ambientLampAnchors.push_back(pair.second);
            pair.first->ApplyLinearImpulse(Vec2f(1.5f, 0),
                                           pair.first->GetPos().AsVec2());
        }
    }

    // Chamber 3 — two heavy hazard lamps. Long rope so the bottom of
    // the swing reaches just above the floor and actually threatens
    // the player. UpdateHazards keeps them swinging at a bounded
    // amplitude.
    const float kHazardRope = kCeilingY - 2.5f;  // ~15.5 with current ceiling
    {
        auto p = buildLamp("pl_lamp_heavy", 28.0f, kHazardRope);
        if (p.first != nullptr) {
            hazardLamps.push_back(p.first);
            hazardLampAnchors.push_back(p.second);
            p.first->ApplyLinearImpulse(Vec2f(45.0f, 0),
                                        p.first->GetPos().AsVec2());
        }
    }
    {
        auto p = buildLamp("pl_lamp_heavy", 38.0f, kHazardRope);
        if (p.first != nullptr) {
            hazardLamps.push_back(p.first);
            hazardLampAnchors.push_back(p.second);
            p.first->ApplyLinearImpulse(Vec2f(-45.0f, 0),
                                        p.first->GetPos().AsVec2());
        }
    }

    // Wire the spotlights once. They'll be repositioned each frame in
    // UpdateLamps to track each lamp's pose.
    if (!ambientLamps.empty()) {
        lamp1Spot.position = ambientLamps[0]->GetPos();
        lamp1Spot.direction.Set(0, -1, 0);
    }
    if (hazardLamps.size() >= 1) {
        lamp2Spot.position = hazardLamps[0]->GetPos();
        lamp2Spot.direction.Set(0, -1, 0);
    }
    if (hazardLamps.size() >= 2) {
        lamp3Spot.position = hazardLamps[1]->GetPos();
        lamp3Spot.direction.Set(0, -1, 0);
    }

    // The ambient point light follows the player every frame (see
    // UpdateCameraFollow) — that's the player's "torch" (light slot 1).
    // The spot light is the closest lamp, picked each frame in
    // UpdateLamps — that's the "atmosphere" (light slot 2). Both are
    // always uploaded simultaneously thanks to the dual-light shader.
    ambientPoint.position.Set(kPlayerSpawnX, 8, 8);
    ambientPoint.color = drash::Color3f(0.85f, 0.9f, 1.05f);
    GetDebugRenderer().SetLight(&ambientPoint);

    // Default to the chamber-2 lamp; UpdateLamps re-picks every frame.
    if (!ambientLamps.empty()) {
        GetDebugRenderer().SetSpotLight(&lamp1Spot);
    } else if (!hazardLamps.empty()) {
        GetDebugRenderer().SetSpotLight(&lamp2Spot);
    }
}

// ----------------------------------------------------------------------------
// Interactive objects (gun, button, trigger zones)
// ----------------------------------------------------------------------------

void Test14::InitInteractive() {
    // Sidearm pickup — small static box on the floor in chamber 1.
    gunPickup = CreateBox("pl_gun", 0.4f, 0.4f,
                          Vec3f(kGunPickupX, 0.4f, 0), false);
    if (gunPickup != nullptr) {
        gunPickup->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other != nullptr && playerObj != nullptr
                    && other->GetSceneObject() == playerObj
                    && !playerHasGun) {
                    EnterState(State::HasGun);
                }
            });
    }

    // Chamber-2 trigger: small box on the floor just inside chamber 2.
    chamber2Trigger = CreateBox(
        "pl_trig2", 0.4f, 0.4f,
        Vec3f(kChamber1Right + 2.5f, 0.4f, 0), false);
    if (chamber2Trigger != nullptr) {
        chamber2Trigger->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other != nullptr && playerObj != nullptr
                    && other->GetSceneObject() == playerObj
                    && state == State::HasGun) {
                    EnterState(State::Chamber2);
                }
            });
    }

    // Climb trigger — fires when the player reaches the chamber-2
    // landing, just to add narration progress.
    climbTrigger = CreateBox(
        "pl_trig_climb", 0.4f, 0.4f,
        Vec3f(15.0f, 11.6f, 0), false);
    if (climbTrigger != nullptr) {
        climbTrigger->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other != nullptr && playerObj != nullptr
                    && other->GetSceneObject() == playerObj
                    && !said_climbHigh) {
                    said_climbHigh = true;
                    Narrate("you made it to the top", 3.5);
                    Narrate("the next door is right beside you", 4.0);
                    Narrate("watch out for heavier lamps ahead", 4.0);
                }
            });
    }

    // Chamber-3 trigger.
    chamber3Trigger = CreateBox(
        "pl_trig3", 0.4f, 0.4f,
        Vec3f(kChamber2Right + 2.5f, 0.4f, 0), false);
    if (chamber3Trigger != nullptr) {
        chamber3Trigger->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other != nullptr && playerObj != nullptr
                    && other->GetSceneObject() == playerObj
                    && state == State::Chamber2) {
                    EnterState(State::Chamber3);
                }
            });
    }

    // Breaker button — chamber 4. Tolerant state check: any progress
    // beyond Intro counts so we don't soft-lock if a chamber-2/3 trigger
    // got skipped (jumped over, or buried under door debris).
    button = CreateBox("pl_button", 0.9f, 0.4f,
                       Vec3f(kButtonX, 0.4f, 0), false);
    if (button != nullptr) {
        button->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other != nullptr && playerObj != nullptr
                    && other->GetSceneObject() == playerObj
                    && state != State::Victory && state != State::Intro) {
                    EnterState(State::Victory);
                }
            });
    }
}

// ----------------------------------------------------------------------------
// Player
// ----------------------------------------------------------------------------

void Test14::InitPlayer() {
    SceneObjectGeometry pg;
    pg.figures.resize(1);
    pg.figures[0].depth = 0.6f;
    pg.figures[0].mass = 1.0f;
    // High friction so the player decelerates fast on landing and
    // doesn't slide off the small stepping-stone platforms. Box2D
    // composes contact friction as sqrt(a * b), and the platforms use
    // the default 1, so 5 here gives an effective ~2.2.
    pg.figures[0].friction = 5.0f;
    pg.figures[0].vertices.push_back(Vec2f( 0.0f,  0.6f));
    pg.figures[0].vertices.push_back(Vec2f( 0.4f, -0.4f));
    pg.figures[0].vertices.push_back(Vec2f( 0.0f, -0.6f));
    pg.figures[0].vertices.push_back(Vec2f(-0.4f, -0.4f));

    PlayerParams pp;
    pp.velocityLimit = 8;
    pp.sceneObjectParams.fixedRotation = true;
    pp.sceneObjectParams.pos.Set(kPlayerSpawnX, 1.0f, 0);

    player = GetPlayersSystem().CreatePlayer(pg, pp);
    if (player != nullptr) {
        playerObj = player->GetSceneObject();

        // Ground-contact counter so we can gate jumps. Filter out
        // projectiles so getting clipped by your own bullet doesn't
        // re-enable jumping mid-air.
        playerObj->AddContactBeginHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other == nullptr) {
                    return;
                }
                SceneObject* o = other->GetSceneObject();
                if (o == nullptr || o == playerObj) {
                    return;
                }
                if (projectileLife.find(o) != projectileLife.end()) {
                    return;
                }
                playerGroundContacts++;
            });
        playerObj->AddContactEndHandler(
            [this](Figure* /*mine*/, Figure* other) {
                if (other == nullptr) {
                    return;
                }
                SceneObject* o = other->GetSceneObject();
                if (o == nullptr || o == playerObj) {
                    return;
                }
                if (projectileLife.find(o) != projectileLife.end()) {
                    return;
                }
                if (playerGroundContacts > 0) {
                    playerGroundContacts--;
                }
            });
    }
}

// ----------------------------------------------------------------------------
// Input
// ----------------------------------------------------------------------------

void Test14::InitProcessors() {
    GetEventSystem().SetMode("powerless");

    GetEventSystem().SetProcessor(
        "C-q", AppEventProcessor([this]() { this->Quit(); }));

    // Custom horizontal movement: bypass PlayerMessage::Left/Right so we
    // can dampen mid-air control. PlayersSystem applies a magnitude-1
    // impulse every frame the key is held (until velocityLimit), which
    // gives near-instant velocity changes mid-jump and feels slippery.
    // On the ground we use roughly the same magnitude; in the air we
    // apply only a small fraction so the player has to commit to a
    // direction before jumping.
    auto walk = [this](float sign) {
        if (playerObj == nullptr) {
            return;
        }
        const float vx = playerObj->GetLinearVelocity().x;
        constexpr float kMaxSpeed = 6.0f;
        // Already moving faster in this direction than the cap?
        // Skip — we don't want to keep accelerating.
        if (sign > 0 && vx >= kMaxSpeed) {
            return;
        }
        if (sign < 0 && vx <= -kMaxSpeed) {
            return;
        }
        const float groundImpulse = 0.6f;
        const float airImpulse = 0.10f;
        const float magnitude =
            (playerGroundContacts > 0) ? groundImpulse : airImpulse;
        playerObj->ApplyLinearImpulse(Vec2f(sign * magnitude, 0),
                                      playerObj->GetMassCenter());
    };

    GetEventSystem().SetProcessor(
        "a", AppEventProcessor([]() {}, [walk]() { walk(-1.0f); }));

    GetEventSystem().SetProcessor(
        "d", AppEventProcessor([]() {}, [walk]() { walk(1.0f); }));

    // Custom jump: PlayerMessage::Jump's impulse is 0.5 (peaks at ~1cm),
    // visually a no-op. Apply a real impulse here, gated on the player
    // having any non-projectile contact (i.e. on the ground or against a
    // wall) and a small cooldown to prevent space-spam.
    GetEventSystem().SetProcessor(
        "SPC", AppEventProcessor([this]() {
            if (playerObj == nullptr) {
                return;
            }
            if (playerGroundContacts <= 0) {
                return;
            }
            if (totalTime - lastJumpTime < 0.35) {
                return;
            }
            playerObj->ApplyLinearImpulse(Vec2f(0, 10.0f),
                                          playerObj->GetMassCenter());
            lastJumpTime = totalTime;
        }));

    GetEventSystem().SetProcessor(
        "LB", AppEventProcessor([this]() {
            if (playerHasGun) {
                SpawnProjectile();
            }
        }));
}

// ============================================================================
// Step / Render
// ============================================================================

void Test14::Step(double _dt) {
    App::Step(_dt);
    stateTime += _dt;
    totalTime += _dt;

    // Emergency-light blink: 2 Hz sine, in [0..1].
    redBlink = 0.5f + 0.5f * std::sin(static_cast<float>(totalTime) * 4.0f);

    UpdateCaptions(_dt);
    UpdateLamps();
    UpdateCameraFollow();
    UpdateProjectiles(_dt);
    UpdateHazards();
    UpdateProgressNarration();
    UpdateStateTransitions();

    // Auto-quit after Victory captions have had time to play.
    if (autoQuitAt > 0.0 && totalTime >= autoQuitAt) {
        Quit();
    }
}

void Test14::UpdateStateTransitions() {
    // Position-based chamber transitions. The contact-zone triggers are
    // unreliable when the player jumps over them or when door debris
    // physically blocks the trigger box. Polling x makes the progression
    // robust.
    if (playerObj == nullptr) {
        return;
    }
    const float px = playerObj->GetPos().x;

    if (state == State::HasGun && px > kChamber1Right + 1.5f) {
        EnterState(State::Chamber2);
    } else if (state == State::Chamber2 && px > kChamber2Right + 1.5f) {
        EnterState(State::Chamber3);
    }
}

void Test14::Render() {
    // The platform clear in main.cpp uses a light-grey colour. Repaint
    // with a near-black tone before App::Render so the rest of the scene
    // reads as a dark interior. We avoid Renderer::Clear here because it
    // also clobbers glViewport (renderer.cpp:268 passes left/bottom/right/
    // top to glViewport which expects x/y/width/height).
    glClearColor(0.04f, 0.04f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    App::Render();
    DrawGlowingMarkers();
    DrawCaptions();
}

// ----------------------------------------------------------------------------
// Step subsystems
// ----------------------------------------------------------------------------

void Test14::UpdateCameraFollow() {
    if (playerObj == nullptr || camera == nullptr) {
        return;
    }
    const float px = playerObj->GetPos().x;
    if (std::fabs(px - cameraTargetX) > cameraDeadband) {
        cameraTargetX = px;
        Vec3f tgt(cameraTargetX, 6, 38);
        camera->GetPos().SetTarget(tgt, 0.6, VarAnimatorBehavior::Single);
    }

    // The point light follows the player so they emit a soft white
    // glow on the surrounding geometry. Slightly above their head so
    // the floor in front catches the diffuse falloff.
    if (playerObj != nullptr) {
        Vec3f pp = playerObj->GetPos();
        ambientPoint.position.Set(pp.x, pp.y + 1.0f, pp.z + 4.0f);
    }
}

void Test14::UpdateLamps() {
    // Blinking red emergency colour. Allowed to exceed 1.0 so the
    // ambient term (which is gAmbientKoeff * gLightColor) reads as a
    // strong red wash even though the ambient coefficient is small.
    const float redIntensity = 0.8f + 1.6f * redBlink;
    const drash::Color3f emergencyColour(redIntensity, 0.05f, 0.05f);

    auto updateSpot = [&](greng::SpotLight& sp, SceneObject* obj) {
        if (obj == nullptr) {
            return;
        }
        sp.position = obj->GetPos();
        const float a = obj->GetAngle().Get() + static_cast<float>(M_PI) * 1.5f;
        sp.direction.Set(std::cos(a), std::sin(a), 0);
        sp.color = emergencyColour;
    };

    if (!ambientLamps.empty()) {
        updateSpot(lamp1Spot, ambientLamps[0]);
    }
    if (hazardLamps.size() >= 1) {
        updateSpot(lamp2Spot, hazardLamps[0]);
    }
    if (hazardLamps.size() >= 2) {
        updateSpot(lamp3Spot, hazardLamps[1]);
    }

    // Pick whichever lamp is closest to the player as the active
    // "atmosphere" light slot 2. This way the red wash always follows
    // the player even though the shader can only carry one spot.
    if (playerObj == nullptr) {
        return;
    }
    const Vec3f pp = playerObj->GetPos();
    SceneObject* best = nullptr;
    greng::SpotLight* bestSpot = nullptr;
    float bestD = 1e30f;
    auto consider = [&](SceneObject* obj, greng::SpotLight* sp) {
        if (obj == nullptr) {
            return;
        }
        const Vec3f op = obj->GetPos();
        const float dx = op.x - pp.x;
        const float dy = op.y - pp.y;
        const float d2 = dx * dx + dy * dy;
        if (d2 < bestD) {
            bestD = d2;
            best = obj;
            bestSpot = sp;
        }
    };
    if (!ambientLamps.empty()) {
        consider(ambientLamps[0], &lamp1Spot);
    }
    if (hazardLamps.size() >= 1) {
        consider(hazardLamps[0], &lamp2Spot);
    }
    if (hazardLamps.size() >= 2) {
        consider(hazardLamps[1], &lamp3Spot);
    }
    if (bestSpot != nullptr) {
        GetDebugRenderer().SetSpotLight(bestSpot);
    }
}

void Test14::UpdateProjectiles(double _dt) {
    for (auto it = projectileLife.begin(); it != projectileLife.end(); ) {
        it->second -= _dt;
        if (it->second <= 0.0) {
            GetScene().DestroyObject(it->first);
            it = projectileLife.erase(it);
        } else {
            ++it;
        }
    }
}

void Test14::UpdateHazards() {
    // Periodic forcing on the hazard lamps to keep them swinging at a
    // bounded amplitude. Box2D damping eats their energy over time, so
    // we add a tiny impulse in the direction of motion when they're
    // moving below the target speed, and a counter-impulse when above
    // it. The window prevents both runaway growth and dead pendulums.
    constexpr float kHazardTargetVx = 5.0f;
    constexpr float kHazardPump = 0.6f;
    for (size_t i = 0; i < hazardLamps.size(); ++i) {
        SceneObject* lamp = hazardLamps[i];
        if (lamp == nullptr) {
            continue;
        }
        const float vx = lamp->GetLinearVelocity().x;
        const float speed = std::fabs(vx);
        if (speed < 0.2f) {
            continue; // near apex — don't fight the pendulum
        }
        const float dir = (vx >= 0.0f) ? 1.0f : -1.0f;
        if (speed < kHazardTargetVx) {
            // Pump in motion direction.
            lamp->ApplyLinearImpulse(Vec2f(dir * kHazardPump, 0),
                                     lamp->GetPos().AsVec2());
        } else if (speed > kHazardTargetVx * 1.2f) {
            // Brake — counter-impulse.
            lamp->ApplyLinearImpulse(Vec2f(-dir * kHazardPump, 0),
                                     lamp->GetPos().AsVec2());
        }
    }

    if (state != State::Chamber3 || playerObj == nullptr) {
        return;
    }
    const Vec3f pp = playerObj->GetPos();
    for (auto* lamp : hazardLamps) {
        if (lamp == nullptr) {
            continue;
        }
        const Vec3f lp = lamp->GetPos();
        const float dx = lp.x - pp.x;
        const float dy = lp.y - pp.y;
        const float d2 = dx * dx + dy * dy;
        if (d2 < 1.7f * 1.7f) {
            playerObj->SetPos(chamber3Spawn);
            playerObj->SetLinearVelocity(Vec2f(0, 0));
            Narrate("crushed try again", 2.5);
            Narrate("watch the swing pattern", 3.0);
            break;
        }
    }
}

void Test14::UpdateCaptions(double _dt) {
    if (captions.empty()) {
        return;
    }
    captions.front().remaining -= _dt;
    if (captions.front().remaining <= 0.0) {
        captions.pop_front();
    }
}

void Test14::UpdateProgressNarration() {
    if (playerObj == nullptr) {
        return;
    }
    const float px = playerObj->GetPos().x;

    // First door: nudge the player after they've had a moment to figure
    // out shooting.
    if (state == State::HasGun && !said_door1Hit && stateTime > 6.0) {
        said_door1Hit = true;
        Narrate("the door is heavy keep shooting", 4.0);
    }

    // Climb chamber: encouragement.
    if (state == State::Chamber2 && !said_climbHigh && px > 8.0f
        && playerObj->GetPos().y > 8.0f) {
        said_climbHigh = true;
        Narrate("almost at the top", 3.0);
    }

    // Chamber 3: warn before each pendulum.
    if (state == State::Chamber3 && !said_lamp2Warn && px > 24.0f) {
        said_lamp2Warn = true;
        Narrate("first pendulum wait for it to swing past", 4.5);
    }
    if (state == State::Chamber3 && !said_lamp3Warn && px > 34.0f) {
        said_lamp3Warn = true;
        Narrate("second one nearly there", 4.0);
    }
    if (state == State::Chamber3 && !said_buttonNear && px > 52.0f) {
        said_buttonNear = true;
        Narrate("the breaker step on it", 4.0);
    }
}

// ----------------------------------------------------------------------------
// Render subsystems
// ----------------------------------------------------------------------------

void Test14::DrawGlowingMarkers() {
    if (camera == nullptr) {
        return;
    }
    auto& r = GetGreng().GetRenderer();

    // Always-glowing bulbs (the lamps' own spotlights point away from
    // them, so without an unlit overlay they'd be black). Modulated by
    // the same blink factor as the spot lights so the visual matches.
    // Also draw the rope from each anchor down to the bulb so the
    // mechanical setup is visible.
    const float bulbIntensity = 0.3f + 0.7f * redBlink;
    const Color4f bulbColour(bulbIntensity, 0.08f, 0.08f, 1.0f);
    const Color4f hazardBulbColour(bulbIntensity, 0.05f, 0.05f, 1.0f);
    const Color4f ropeColour(0.95f, 0.95f, 0.95f, 1.0f);

    for (size_t i = 0; i < ambientLamps.size(); ++i) {
        SceneObject* lamp = ambientLamps[i];
        if (lamp == nullptr) {
            continue;
        }
        if (i < ambientLampAnchors.size()) {
            r.DrawLine(*camera, ambientLampAnchors[i], lamp->GetPos(),
                       2.0f, ropeColour, true);
        }
        r.DrawPoint(*camera, lamp->GetPos(), 30, bulbColour, true);
    }
    for (size_t i = 0; i < hazardLamps.size(); ++i) {
        SceneObject* lamp = hazardLamps[i];
        if (lamp == nullptr) {
            continue;
        }
        if (i < hazardLampAnchors.size()) {
            r.DrawLine(*camera, hazardLampAnchors[i], lamp->GetPos(),
                       2.5f, ropeColour, true);
        }
        r.DrawPoint(*camera, lamp->GetPos(), 42, hazardBulbColour, true);
    }

    // Player marker — small bright dot so they read in the dark
    // without blinding the rest of the scene. The white point light
    // (in chambers without a lamp) does the actual illumination.
    if (playerObj != nullptr) {
        const Vec3f pp = playerObj->GetPos();
        r.DrawPoint(*camera, pp, 10,
                    Color4f(0.9f, 0.95f, 1.0f, 1.0f), true);
    }

    // Glowing pickups.
    if (gunPickup != nullptr && !playerHasGun) {
        r.DrawPoint(*camera, gunPickup->GetPos(), 22,
                    Color4f(0.55f, 0.9f, 1.0f, 1.0f), true);
    }
    if (button != nullptr && state != State::Victory) {
        r.DrawPoint(*camera, button->GetPos(), 24,
                    Color4f(0.4f, 1.0f, 0.45f, 1.0f), true);
    }

    // Projectile tracers — bright unlit dots over the dynamic body so
    // they read clearly against the dark scene.
    for (auto& kv : projectileLife) {
        SceneObject* p = kv.first;
        if (p == nullptr) {
            continue;
        }
        r.DrawPoint(*camera, p->GetPos(), 14,
                    Color4f(1.0f, 0.95f, 0.8f, 1.0f), true);
    }

    // Victory afterglow — extra unlit highlights to fake "the lights
    // came back on".
    if (state == State::Victory) {
        for (float x = kChamber1Left + 4; x < kChamber4Right; x += 6.0f) {
            r.DrawPoint(*camera, Vec3f(x, kCeilingY - 1.0f, 0), 14,
                        Color4f(0.85f, 0.95f, 1.0f, 1.0f), true);
        }
    }
}

void Test14::DrawCaptions() {
    if (captions.empty() || uiCamera == nullptr) {
        return;
    }
    auto& r = GetGreng().GetRenderer();

    // The ortho ui camera spans (-kUiOrthoHalf .. +kUiOrthoHalf) horizontally
    // and (-halfH .. +halfH) vertically, where halfH depends on aspect ratio.
    // We want the caption near the bottom of the visible area.
    const float aspect = GetGreng().GetViewport().GetAspectRatio();
    const float halfH = kUiOrthoHalf / (aspect != 0.0f ? aspect : 1.0f);

    const Caption& c = captions.front();
    Vec2f sz(kCaptionCharW, kCaptionCharH);

    // Compute the real rendered width by summing per-character advances.
    auto computeWidth = [&](float charScale) {
        float w = 0.0f;
        for (char ch : c.text) {
            w += charScale * captionCharAdvance(ch);
        }
        return w;
    };

    float textW = computeWidth(sz.x);

    // If the caption is wider than 90% of the screen, scale it down
    // proportionally so it always fits.
    const float maxW = kUiOrthoHalf * 1.8f;
    if (textW > maxW) {
        const float scale = maxW / textW;
        sz.x *= scale;
        sz.y *= scale;
        textW *= scale;
    }

    const Vec2f pos(-textW * 0.5f, -halfH + kCaptionMarginBottom);

    // DrawChar inherits glColor from whatever the last GL call set, so
    // force a bright opaque colour for readability before each draw.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 0.85f, 1.0f);
    r.DrawString(*uiCamera, true, pos, sz, false, c.text);
    glLineWidth(1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

// ============================================================================
// Game state
// ============================================================================

void Test14::EnterState(State _s) {
    state = _s;
    stateTime = 0.0;
    switch (_s) {
    case State::HasGun:
        playerHasGun = true;
        if (gunPickup != nullptr) {
            GetScene().DestroyObject(gunPickup);
            gunPickup = nullptr;
        }
        Narrate("sidearm acquired", 4.0);
        Narrate("left click to aim and shoot", 4.0);
        Narrate("break the door ahead", 5.0);
        break;

    case State::Chamber2:
        if (chamber2Trigger != nullptr) {
            GetScene().DestroyObject(chamber2Trigger);
            chamber2Trigger = nullptr;
        }
        Narrate("you are through", 3.0);
        Narrate("a swinging emergency lamp ahead", 4.0);
        Narrate("climb the platforms to reach the next door", 4.5);
        Narrate("the door above must be shot from up there", 4.5);
        break;

    case State::Chamber3:
        if (chamber3Trigger != nullptr) {
            GetScene().DestroyObject(chamber3Trigger);
            chamber3Trigger = nullptr;
        }
        // Fixed safe respawn point on the chamber-3 floor — capturing
        // the player's actual position would put them mid-air if they
        // entered the chamber by falling through the broken door.
        chamber3Spawn.Set(kChamber2Right + 2.5f, 1.0f, 0.0f);
        Narrate("the corridor", 3.0);
        Narrate("heavy lamps are swinging across", 4.0);
        Narrate("they will crush you", 3.5);
        Narrate("time your run between the swings", 4.5);
        break;

    case State::Victory:
        if (button != nullptr) {
            GetScene().DestroyObject(button);
            button = nullptr;
        }
        if (camera != nullptr) {
            Vec3f tgt(kButtonX - 4, 8, 30);
            camera->GetPos().SetTarget(tgt, 3.0,
                                       VarAnimatorBehavior::Single);
        }
        Narrate("the breaker clicks back into place", 4.0);
        Narrate("power restored", 4.0);
        Narrate("welcome back", 4.0);
        Narrate("press ctrl q to exit", 10.0);
        // Auto-quit ~24s after entering Victory so the user gets
        // closure even if the captions are missed.
        autoQuitAt = totalTime + 24.0;
        break;

    default:
        break;
    }
}

void Test14::Narrate(const std::string& _text, double _duration) {
    captions.push_back({ _text, _duration, _duration });
}

void Test14::SpawnProjectile() {
    if (playerObj == nullptr || camera == nullptr) {
        return;
    }

    // Cast a ray from the cursor through the world XY plane to figure
    // out where the player is aiming.
    Plane pl(PlaneXY);
    Vec3f worldHit;
    camera->CastRay(GetCursorPos(), pl, worldHit);

    Vec2f dir(worldHit.x - playerObj->GetPos().x,
              worldHit.y - playerObj->GetPos().y);
    if (dir.LengthSquared() < 0.001f) {
        return;
    }
    dir.Normalize();

    // Build a tiny dynamic box and shoot it from in front of the player.
    SceneObjectGeometry g;
    g.figures.resize(1);
    g.figures[0].depth = 0.2f;
    g.figures[0].mass = 0.5f;
    g.figures[0].vertices.push_back(Vec2f(-0.12f,  0.12f));
    g.figures[0].vertices.push_back(Vec2f( 0.12f,  0.12f));
    g.figures[0].vertices.push_back(Vec2f( 0.12f, -0.12f));
    g.figures[0].vertices.push_back(Vec2f(-0.12f, -0.12f));

    SceneObjectParams p;
    Vec3f start = playerObj->GetPos();
    start.x += dir.x * 0.9f;
    start.y += dir.y * 0.9f + 0.2f;
    p.pos = start;
    p.dynamic = true;
    p.fixedRotation = false;

    SceneObject* proj = GetScene().CreateObject(g, p);
    if (proj == nullptr) {
        return;
    }
    proj->SetLinearVelocity(Vec2f(dir.x * 38.0f, dir.y * 38.0f));
    projectileLife[proj] = 2.0;
}

} // namespace test
} // namespace drash
