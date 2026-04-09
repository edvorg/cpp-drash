# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

`drash` is a 2D/3D game engine focused on a destruction system. Box2D 2.x is used for physics and is vendored under `3rd/Box2D`. Two other deps (`3rd/react`, `3rd/tuple`) are git submodules — run `git submodule update --init` after fresh clones.

## Build

The repo has two parallel build systems that must be kept in sync when source files are added/removed:

- **`Makefile`** — primary build for the SDL target. Produces an executable named `drash` in the repo root.
- **`drash.qtproject/drash_sdl.pro`** + **`drash.qtproject/drash.pro`** — qmake projects. `drash_sdl.pro` mirrors the Makefile's SDL build; `drash.pro` is the Qt-based editor build (pulls in `src/qt/`, `src/editor/`, `src/explosion/`) which is **not** built by the Makefile.

Common commands:

```bash
make -j$(nproc)              # build SDL target
make clean                   # remove .o files under src/
make check-syntax CHK_SOURCES=src/path/to/file.cpp   # syntax-only check (used by editor flymake)
make print-cflags            # dump compile flags
```

Compiler flags from the Makefile: `clang++ -std=c++1y -stdlib=libc++ -O3 -Wall -DDRASH_DEBUG -I3rd`. The SDL build always defines `DRASH_DEBUG`; the qmake build toggles `DRASH_DEBUG`/`DRASH_RELEASE` by configuration.

Runtime deps (linked): `rt GL SDL GLEW SDL_image openal vorbis vorbisfile c++abi pthread`. Note: SDL **1.2**, not SDL2.

**When adding a new `.cpp` file** under `src/`, append it to *both* `Makefile` (`SOURCES +=`) and `drash.qtproject/drash_sdl.pro` (`SOURCES +=` and the matching header to `HEADERS +=`). Forgetting one causes link or IDE failures depending on which build is used.

## Run

The binary has no menu — it launches a single "test" scene selected from the command line:

```bash
./drash --test test1     # ... through test13
```

`src/sdl/main.cpp` parses `--test <name>` and dispatches to `drash::test::StartApp()` in `src/test/test.cpp`, which is a hand-written `if/else` chain mapping each name to a `TestN` class. **To add a test scene**, create `src/test/testN.{h,cpp}` subclassing `drash::App`, add it to the dispatcher in `src/test/test.cpp`, and add the `.cpp` to both build files.

There is no unit test framework in the repo — `test1`..`test13` are interactive demos / smoke scenes, not automated tests.

Resources (`res/shaders`, `res/assets`, `res/demo_level.dlvl`) are loaded by relative path, so run `./drash` from the repo root.

## Architecture

The engine is organized into a few self-contained subsystems wired together by a single `App` god-object. The big picture:

- **`src/app/App`** owns one of every major subsystem and drives them from `Step(dt)` / `Render()`. Concrete games/demos subclass `App` (see any `src/test/testN.h`) and override `Step` / `Render`. The platform layer (`src/sdl/main.cpp`) constructs a `Greng`, calls the test factory to get an `App*`, and runs the main loop. There is also a Qt platform layer under `src/qt/` + `src/editor/` used only by the qmake `drash.pro` build.

- **`src/greng/` (namespace `greng`)** — the rendering engine, deliberately decoupled from `drash`. The `Greng` class aggregates manager objects (`MeshManager`, `TextureManager`, `VertexShaderManager`, `FragmentShaderManager`, `ShaderProgramManager`, `FrameBufferManager`, `CameraManager`) plus a `Renderer` and `Viewport`. Resources are owned by their managers; everything else holds raw pointers into them. `App` keeps a *reference* to an externally-constructed `Greng` because GL context lifetime is owned by the platform layer (SDL or Qt).

- **`src/scene/`** — `Scene` wraps a `b2World` and owns `SceneObject`s; each object has one or more `Figure`s (Box2D fixtures + extruded 3D geometry — this is where the destruction system lives). `GeometryManager` stores reusable `SceneObjectGeometry` templates that the level system instantiates into the scene.

- **`src/players/PlayersSystem`**, **`src/levelmanager/LevelManager`**, **`src/ui/UISystem`**, **`src/debugrenderer/DebugRenderer`** — additional subsystems aggregated by `App`. `DebugRenderer` is the primary visualization for the SDL build; `App::Render()` only calls `debugRenderer.Render()` + UI debug draw, so test apps that need full 3D rendering do their own draws inside `Render()`.

- **`src/app/AppEventSystem`** — input abstraction. The platform layer translates SDL/Qt events into `EventKey` enum values via `BeginEvent` / `EndEvent`, and `AppEventProcessor` matches key combinations and fires callbacks. Test scenes register processors in their constructor (see `Test1::SetProcessors`).

- **`src/misc/`** — math primitives (`Vec2/3/4`, `Matrix4`, `Ray`, `Plane`), animation helpers, and `objectfactory.h` (a templated pool used by managers). Headers only, mostly.

- **`src/diag/`** — `LOG_INFO/LOG_ERR/LOG_WARN` macros and a `Timer`. Use these instead of `std::cout`.

- **`src/trash/`** — historical/experimental code (e.g. `alsound.cpp`); pulled into the build but not on any active code path. Don't add new code here.

## Code style

`.clang-format` (LLVM-based, 4-space indent, 80-col, attached braces, namespace contents indented). Headers use `#pragma once` AND traditional include guards. Source files start with a `// DRASH_LICENSE_BEGIN ... // DRASH_LICENSE_END` GPLv3 header — preserve it when editing existing files and copy it into any new files.
