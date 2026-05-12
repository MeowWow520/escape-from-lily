# escape-from-lily

C++20 escape game using SDL3, CMake, vcpkg. Windows-only with MSVC. Built in CLion 2026.1.1.

## Build

CMake options (required — vcpkg manifest mode with static triplet):

```
-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
-DVCPKG_FEATURE_FLAGS=manifests
-DVCPKG_TARGET_TRIPLET=x64-windows-static
```

Build target: `escape-from-lily`. Output lands in `cmake-build-debug-visual-studio/`.

After build, the `assets/` folder is automatically copied to the output directory (POST_BUILD command in CMakeLists.txt).

Dependencies (managed by vcpkg): SDL3, SDL3_image, SDL3_mixer, SDL3_ttf, glm.

## Code conventions

- **C++20**, MSVC compiler with `/source-charset:utf-8 /execution-charset:utf-8`
- **4-space indent**, no tabs
- **Member variables**: `m_` prefix, snake_case (e.g. `m_window_size`, `m_current_scene`)
- **Class names**: PascalCase (e.g. `TexturedEntity`, `SceneMain`)
- **Include guards**: `ESCAPE_FROM_LILY_<CLASSNAME>_H`
- **Each class**: one `.h` + one `.cpp`, filename matches class name
- **Comments in Chinese**, Doxygen `/** */` style for public API
- **File header**: `// Created by MeowWow520 on YYYY/M/D`
- **Pointer safety**: `SDL_Window*`/`SDL_Renderer*` are raw pointers owned by SDL; `SDL_Texture*` uses `std::unique_ptr` with `SDL_DestroyTexture` deleter
- **Logging**: use `SDL_Log("[tag] message")` — do not use `printf` or `std::cout`

NOTE: The `docs/CONTRIBUTING.md` style guide claims `camelCase` variables and `_` suffix for members, but the **actual codebase** uses `m_` prefix with snake_case. Follow the actual code, not the doc.

## Architecture

```
Game (singleton, main loop)
  └─ SceneMain (current scene, created in Game::Initialize)
       └─ Background (TexturedEntity)

Object               ← base: holds Game& reference, virtual lifecycle methods
  ├─ ObjectWorld     ← adds m_world_pos
  │    └─ ObjectScreen   ← adds m_screen_pos (screen coordinates)
  │         └─ TexturedEntity ← texture, rotation, scale, pivot, blend mode
  │              └─ Background
  └─ Scene           ← adds m_world_size, m_camera_pos, TransScreenPos()
       └─ SceneMain  ← main gameplay scene
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

**COLOR macro** (`Def.h`): `COLOR(0xAABBCCDD)` expands to `AA, BB, CC, DD` (RGBA components separated for SDL_Color).

## How to add a new entity

1. Create `.h`/`.cpp` in `src/core/Entities/`
2. Inherit from `TexturedEntity` (or `ObjectScreen`/`ObjectWorld` if no texture needed)
3. Register the new `.cpp` in `CMakeLists.txt` under `add_executable`
4. Override lifecycle methods as needed; chain to parent class for default behavior

## How to add a new scene

1. Create `.h`/`.cpp` in `src/` (not under `core/`)
2. Inherit from `Scene`
3. Register in `CMakeLists.txt`
4. Instantiate in `Game::Initialize()` (replace or add scene switching logic)

## No CI / no tests

There are no unit tests, no CTest, no GitHub Actions. Qodana config (`qodana.yaml`) exists but is not wired into any CI pipeline. Manual build-and-run is the only verification step.

## Git

- `.idea/` is in `.gitignore` — IDE settings are not shared
- `CLAUDE.md` is in `.gitignore` — does not ship with the repo
- Commit convention: Conventional Commits with custom `upd` type (see `docs/CONTRIBUTING.md`)
- Branch naming: `feature/*`, `fix/*`, etc.
