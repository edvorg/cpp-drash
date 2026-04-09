# Powerless — single-level scenario design

A 5-10 minute side-scrolling rescue through a darkened maintenance bay.
The reactor is offline; only emergency lamps swinging from ropes light
the dark. The player wakes up trapped, finds a sidearm, blasts through
destructible debris, dodges falling pendulum lamps, and reaches the
breaker room to restore power.

The scenario is implemented as a single test scene
(`src/test/test14.{h,cpp}`, run with `./drash --test test14`). All
existing tests are unaffected.

## Why this scenario fits the drash engine

Every gameplay element maps to a primitive that already works in another
test scene — nothing depends on bit-rotted features (audio, UI widgets,
particles, sensor fixtures, prefabs).

| Beat | Engine primitive | Existing precedent |
|---|---|---|
| Destructible walls | `Object1` template + `SceneObjectGeometry::ComputeDestructionGraph(0.5)` | `src/test/test6.cpp:179` |
| Player + Left/Right/Jump | `PlayersSystem::CreatePlayer` + `SendMessage(PlayerMessage::Jump)` | `src/test/test6.cpp:229,344-347` |
| Shooting | `LB` → spawn small dynamic box, `SetLinearVelocity` toward cursor | `src/test/test6.cpp:341-378` |
| Swinging lamp | Dynamic `lamp` `SceneObject` + `Scene::CreateJointRope` + `SpotLight` chasing it | `src/test/test10.cpp:128-177` |
| Lamp glow visible | `Renderer::DrawPoint` overlay so the bulb stays lit even though its own spotlight points away | `src/test/test10.cpp:54-72` |
| Narration captions | `Renderer::DrawString` against an ortho UI camera | `src/test/test11.cpp:90-98` |
| Trigger zones | Tiny static figures with contact handlers, destroyed on first contact | `src/test/test6.cpp:196-208` |
| Camera follow | `camera->GetPos().SetTarget(target, 0.5, VarAnimatorBehavior::Single)` only when player has moved past a deadband | `src/test/test6.cpp:106-111` |
| Hazard respawn | `playerObj->SetPos(spawn)` + `SetLinearVelocity({0,0})` from a contact handler | n/a — engine has no checkpoint system |

## Layout (side-on, left to right)

```
                                                                EXIT
                                                                ▒▒▒
                                                              ┌─┘
                                                              │  ← weak wall
       Lamp1                Lamp2     Lamp3                   │
        ┃ ┃                  ┃ ┃      ┃ ┃                     │
        ┃ ┃     ┌──┐         ┃ ┃      ┃ ┃                     │
        ●●     ┌──┐ ┌──┐    ●●        ●●                      │
              ╲     ╲    ╲            ↔ swinging              │
              step3 step2 step1                               │
   Gun                                                        Btn
═══════════════════════════════════════════════════════════════════
Ch.1 (intro)│ Ch.2 (climb)        │ Ch.3 (gauntlet)          │ Ch.4
   −30..−10 │  −10..18            │  18..50                   │ 50..65
            │                     │                           │
            ▲ door1               ▲ door2                     ▲ door3
              (destructible)        (destructible)              (destructible)
```

All four chambers share one continuous floor and ceiling. Travel is
horizontal. Each chamber is gated by a destructible vertical `Object1`
that the player can shoot apart.

## State machine

```
   Intro ──pickup──▶ HasGun ──trigger──▶ Chamber2 ──trigger──▶ Chamber3 ──button──▶ Victory
```

| State | Means | Side effects on enter |
|---|---|---|
| `Intro` | Player just woke up. No gun. LB does nothing. | Push intro narration. |
| `HasGun` | Walked over the sidearm. LB now spawns projectiles. | Destroy pickup object, push tutorial narration. |
| `Chamber2` | Walked past chamber-1 trigger zone. | Destroy trigger, retarget ambient light to lamp 1, push climbing narration. |
| `Chamber3` | Walked past chamber-2 trigger zone. | Destroy trigger, store respawn point, push warning narration. |
| `Victory` | Walked onto the breaker button. | Destroy button, retarget camera, push outro narration. |

## Beat-by-beat playthrough (~7 min)

**Chamber 1 — Wakeup (~1 min, tutorial)**

- Player spawns near `x=-25`. Camera locked. Single dim `PointLight`
  overhead.
- Captions queue:
  1. `"...power's out."` (4 s)
  2. `"Get up. Find a way out."` (4 s)
  3. `"Move with A and D. Jump with SPACE."` (5 s)
- Walking right, the player runs into a glowing point on the floor at
  `x=-15` — the sidearm. Walking onto it fires its contact handler:
  `state = HasGun`, the pickup is destroyed, captions:
  1. `"Sidearm. Make them count."` (4 s)
  2. `"Left-click to shoot."` (4 s)
  3. `"Break the door ahead."` (5 s)
- The door at `x=-10` is a destructible `Object1` rotated 90°. Six to
  eight projectiles snap enough figures off its destruction graph to
  open a player-sized gap.
- Walking through fires the chamber-2 trigger zone right after the door.

**Chamber 2 — The lamp (~2 min, climbing puzzle)**

- The room's only lit feature is a swinging lamp on a rope at the
  ceiling. Same pattern as `test10.cpp:128-177`: a small static "anchor"
  `SceneObject` at the ceiling, a small dynamic `lamp` `SceneObject` 5 m
  below it, joined with `Scene::CreateJointRope`. The lamp's `SpotLight`
  position and direction track the lamp's pose every frame.
- Captions:
  1. `"Climb. Something glows above the rubble."` (5 s)
  2. `"The platforms might give way if you shoot them."` (5 s)
- Three small static stepping-stone platforms (`step1`, `step2`,
  `step3`) form broken stairs. The player can jump them directly, or
  shoot the platform supports to drop them.
- Periodic narration as the player climbs: `"Higher."`, `"The lamp is
  swinging — it's fine for now."`
- At the top of the climb, the player passes through door 2 and triggers
  the chamber-3 zone.

**Chamber 3 — Pendulum gauntlet (~2-3 min, set piece)**

- Long horizontal corridor with two heavy lamps on long ropes
  (`lamp_heavy` geometry, mass 4). They swing perpendicular to player
  travel.
- Initial impulses are applied at construction (`ApplyLinearImpulse`) to
  start them swinging.
- Captions on entry:
  1. `"Don't get crushed."` (3 s)
  2. `"Time the gaps."` (4 s)
- Each `Step()` checks player–lamp distance. If within 1.5 units of any
  hazard lamp, teleport the player back to `chamber3Spawn`, zero
  velocity, push `"Crushed. Try again."`
- After both swinging lamps, door 3 leads to chamber 4.

**Chamber 4 — Power restored (~1 min, climax + outro)**

- Small room with a glowing button (`button`) at `x=62`.
- Walking onto the button fires its contact handler: `state = Victory`,
  destroy button, retarget camera to `(roomCenter, 8, 25)` over 3 s,
  retarget the ambient `PointLight` upward.
- Captions:
  1. `"Power restored."` (4 s)
  2. `"Welcome back."` (4 s)
  3. `"Press C-q to exit."` (10 s)

## Narration philosophy

Narration is the *primary* feedback channel. The user must always know
**what they did**, **where they are**, and **what to try next**. The
caption queue is a FIFO drained by the `Step()` loop — a new caption
becomes visible as soon as the previous one expires, so pushing several
in a row plays them sequentially without overlap.

Narration triggers fall in three buckets:

1. **State transitions** — every state change pushes 2–4 captions
   describing what just happened and what to do next.
2. **Discovery** — walking onto a glowing object pushes a caption naming
   it and explaining what it does.
3. **Implicit progress markers** — passing certain x-positions or
   shooting certain walls pushes encouragement
   (`"That's it — keep at it."`, `"Almost through."`).

Captions render in the bottom centre of the screen via the ortho
`uiCamera` at `Renderer::DrawString` size `(1.5, 1.5)`. The bitmap font
is uppercase-only and limited to roughly 40 characters per line —
captions are written to fit.

## Required content additions

- **No edits** to `templates.txt`. The level reuses `Object1` from the
  shipped templates for destructible walls.
- **No new `.dlvl` file.** Static walls/floors are built procedurally in
  `Test14::InitWorld()`, modeled after `test10.cpp:83-178`. Building
  geometry in code keeps the level data, gameplay triggers, and contact
  handlers all in one file, which is easier to iterate on than a
  text-format level.
- **One new test scene**: `src/test/test14.{h,cpp}`, dispatched from
  `src/test/test.cpp`, added to both `Makefile` and
  `drash.qtproject/drash_sdl.pro`.

## Risks and gotchas

These are the things that will bite you when iterating on the scene.
They were captured during the engine inventory pass and from existing
test code.

1. **`SetTarget` thrash.** If the camera follow code calls
   `camera.GetPos().SetTarget(...)` every frame with a moving target,
   the `VarAnimator` restarts each call and never converges. Gate on a
   deadband: only retarget when the player has moved more than ~2 units
   since the last retarget. Test6:106-111 does this correctly.

2. **Trigger zones aren't sensors.** The engine doesn't expose
   `b2Fixture::SetSensor`, so a "walk-over trigger" is a real solid
   figure that the player physically collides with. Make it small, put
   it on the floor in a wide passage, and `Scene::DestroyObject` it
   from inside the contact handler so it doesn't become an obstacle.
   Don't put a trigger in a tight gap.

3. **Destruction tuning.** `ComputeDestructionGraph(0.5)` works for
   `Object1` in the demo level but is fragile to vertex spacing. Loaded
   templates only — calling it on procedurally-built single-figure
   geometry has no effect (one figure, nothing to detach).

4. **Projectile lifetime.** Test6 spawns projectiles and never cleans
   them up. Test14 keeps a `std::map<SceneObject*, double>` of
   per-projectile remaining seconds and explicitly
   `Scene::DestroyObject`s them after 2 s, both to keep the scene clean
   and to avoid the `b2World` crawling.

5. **Bitmap font width.** The `DrawString` font is fixed-width and
   uppercase-only. Captions must be ≤ 40 characters per line and
   should not contain unsupported glyphs (lowercase punctuation may not
   render). Test11:92-97 is the only example call — copy its argument
   shape verbatim.

6. **Respawn = teleport.** No checkpoint or save system exists.
   "Dying" in chamber 3 means
   `playerObj->SetPos(chamber3Spawn)` + `SetLinearVelocity({0,0})`. If
   you forget to zero velocity the player carries the lamp's impact
   into respawn and ragdolls off-screen.

7. **Lights are global, not per-object.** `DebugRenderer` holds one
   `PointLight*` and one `SpotLight*`. To create the impression of
   "lights coming on" for the Victory beat, draw extra unlit overlays
   manually with `DrawPoint`/`DrawTriangle` in `Render()` instead of
   trying to add more lights to the renderer.

8. **`SceneObject::GetPos()` returns `const Vec3f&`.** It does *not*
   return the animator. To `.SetTarget(...)` a scene object's position
   you need `GetPosXY()` (returns `Animator<Vec2f>&`) or `GetPosZ()`
   (returns `Animator<float>&`). The camera is different — its
   `GetPos()` returns the `VarAnimator<Var<Vec3f>>` directly.

9. **Two animator types, two enums.** Drash's `Animator<T>` (for
   `SceneObject`) uses `AnimatorBehavior::{Single,Loop,Bounce}`. Greng's
   `Camera::Get*()` use `VarAnimator<Var<T>>`, which uses
   `VarAnimatorBehavior::{Single,Loop,Bounce}` from the in-tree react
   shim. Mixing them is a compile error.

## Optional stretch features (skipped in v1)

- **Health/ammo HUD** — extra `DrawString` calls in `Render()` rendering
  `"AMMO 5/8"`. Would need an `int ammo` member and a refill mechanic.
- **Animated door** — `SceneObject::GetPosXY().SetTarget(...)` a
  kinematic door upward when the button is pressed.
- **Screen shake on impact** — `src/misc/screenshaker.hpp` is
  implemented but never exercised by any test. Risk of compile/runtime
  surprises; deferred.

## Run

```bash
cd res && ../drash --test test14
```

Use the **left** Ctrl/Alt/Shift modifier — the SDL key map only honours
left-hand variants. Quit with `C-q`.
