# escape-from-lily

C++20 escape game using SDL3, CMake, vcpkg. Windows-only with MSVC. Built in CLion 2026.1.1.

## Build

### Via CMakePresets (recommended)

Presets `debug` and `release` are defined in `CMakePresets.json`. They require `VCPKG_ROOT` environment variable pointing to your vcpkg installation.

```powershell
# Configure
cmake --preset debug
# Build
cmake --build --preset debug
```

Binary output lands in `build/Debug/`. The `assets/` folder is auto-copied to the output directory (POST_BUILD).

### Manual CMake

CMake options (required — vcpkg manifest mode, static triplet):

```
-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
-DVCPKG_FEATURE_FLAGS=manifests
-DVCPKG_TARGET_TRIPLET=x64-windows-static
```

Build target: `escape-from-lily`.

### Dependencies (vcpkg)

SDL3, SDL3_image, SDL3_mixer, SDL3_ttf, glm, spdlog, fmt.

`spdlog` is **the** logging library used throughout — do not use `printf`, `std::cout`, or `SDL_Log`.

## Code conventions

- **C++20**, MSVC with `/utf-8` (set in CMakeLists.txt)
- **4-space indent**, no tabs
- **Member variables**: `m_` prefix, snake_case (e.g. `m_window_size`, `m_current_scene`)
- **Class names**: PascalCase (e.g. `TexturedEntity`, `SceneMain`)
- **Include guards**: `ESCAPE_FROM_LILY_<CLASSNAME>_H`
- **Each class**: one `.h` + one `.cpp`, filename matches class name. Some base/utility classes are header-only (no `.cpp`).
- **Comments in Chinese**, Doxygen `/** */` style for public API
- **File header**: `// Created by MeowWow520 on YYYY/M/D`
- **Pointer safety**: `SDL_Window*`/`SDL_Renderer*` are raw pointers owned by SDL; `SDL_Texture*` uses `std::unique_ptr` with `SDL_DestroyTexture` deleter (aliased as `TexturePtr` in `TexturedEntity`)
- **Logging**: use `spdlog` — `spdlog::info("message")`, `spdlog::error("message")`. Do NOT use `printf`, `std::cout`, or `SDL_Log`.

NOTE: `docs/CONTRIBUTING.md` claims `camelCase` variables and `_` suffix for members — the **actual codebase** uses `m_` prefix with snake_case. Follow the actual code, not the doc.

### Def.h key utilities

- `COLOR(0xAABBCCDD)` → expands to `AA, BB, CC, DD` (separated RGBA for `SDL_Color`)
- `SDL_LibInitChecker(bool flag, string name)` — checks SDL subsystem init, logs result
- `EFL_ClassInit(int return_code, source_location)` / `EFL_ClassQuit(...)` — logging wrappers for class lifecycle error handling
- `EFL_Vec2AddToRect(vec2 pos, vec2 size)` / `EFL_Vec2AddToRectFloat` — glm vec2 to SDL_Rect/SDL_FRect
- `CLR_RED`, `CLR_BLUE` etc. — ANSI color escape codes for log output
- `using ssl = std::source_location;` alias

## Architecture

```
Game (singleton, main loop)
  └─ SceneMain (current scene, created in Game::Initialize)
       ├─ Background (TexturedEntity)
       └─ uses Input/KeyboardInput for input handling

Object               ← base: holds Game& reference, virtual lifecycle methods
  ├─ ObjectWorld     ← adds m_world_pos
  │    └─ ObjectScreen   ← adds m_screen_pos (screen coordinates)
  │         └─ TexturedEntity ← texture, rotation, scale, pivot, blend mode
  │              ├─ Background
  │              ├─ UserInterface
  │              └─ MovableEntity ← movement flags, max speed, acceleration
  └─ Scene           ← adds m_world_size, m_camera_pos, TransScreenPos()
       └─ SceneMain  ← main gameplay scene
  └─ Input           ← (standalone) input handling base
       └─ KeyboardInput
```

**Lifecycle pattern** (all classes follow this):
1. Constructor — initializes members
2. `Initialize()` — creates SDL resources, returns 0 on success, -1 on failure
3. `HandleEvents(SDL_Event)` / `Update(float dt)` / `Render()` — per-frame
4. `Quit()` — cleanup, returns 0 on success

**Game singleton**: `Game::GetInstance()` returns the single instance. Copy/assign deleted. Main loop in `Game::Running()`:
- Poll events → dispatch to current scene's HandleEvents
- Call Update with delta time
- Call Render (clears with `COLOR(0xFF006EFF)`, renders scene, presents)

**Coordinate systems**: World position (`m_world_pos`) → screen position (`m_screen_pos = world_pos - camera_pos`). Camera defaults to world center.

## How to add a new entity

1. Create `.h`/`.cpp` in `src/core/Entities/`
2. Inherit from `TexturedEntity`, `MovableEntity`, or `ObjectScreen`/`ObjectWorld` if no texture needed
3. Register the new `.cpp` in `CMakeLists.txt` under `add_executable`
4. Override lifecycle methods as needed; chain to parent class for default behavior

## How to add a new scene

1. Create `.h`/`.cpp` in `src/` (not under `core/`)
2. Inherit from `Scene`
3. Register in `CMakeLists.txt`
4. Instantiate in `Game::Initialize()` (replace or add scene switching logic)

## CI

GitHub Actions workflow at `.github/workflows/ci-windows.yml` builds on push/PR to `main`/`master`/`develop`. Uses CMakePresets (`debug` preset). The "Run tests" step is disabled (`if: false`) — there are no tests yet. Qodana config (`qodana.yaml`) exists but is not wired into CI.

Manual build-and-run is the primary verification method.

## Git

- `.idea/` and `.opencode/` are in `.gitignore` — IDE settings and OpenCode config are not shared
- `CLAUDE.md` is in `.gitignore` — does not ship with the repo
- Commit convention: Conventional Commits with custom `upd` type (see `docs/CONTRIBUTING.md`)
- Branch naming: `feature/*`, `fix/*`, etc.
