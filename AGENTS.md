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

SDL3, SDL3_image (png+jpeg features), SDL3_mixer, SDL3_ttf, glm, spdlog, fmt, nlohmann-json.

`spdlog` is **the** logging library used throughout — do not use `printf`, `std::cout`, or `SDL_Log`. Prefer the `EFL_LOGGER_INFO`/`EFL_LOGGER_ERROR` macros when possible.

spdlog is linked via `find_package(spdlog CONFIG REQUIRED)` + `target_link_libraries(... spdlog::spdlog)` in CMakeLists.txt.

`nlohmann-json` is header-only — it appears in `vcpkg.json` and is `#include`d by `ConfigManager.cpp`, but does not need a `find_package` or `target_link_libraries` entry.

## Code conventions

- **C++20**, MSVC with `/utf-8`, `/Zc:preprocessor`, `/Zc:__cplusplus` (set in CMakeLists.txt)
- **4-space indent**, no tabs
- **Member variables**: `m_` prefix with Hungarian-like type hints — `m_fMaxSpeed`, `m_vec2_worldPos`, `m_bVisible`, `m_strTitle`, `m_i32FPS`, `m_cEntityName`, `m_pRenderer`
- **Class names**: PascalCase (e.g. `TexturedEntity`, `SceneMain`)
- **Include guards**: `ESCAPE_FROM_LILY_<CLASSNAME>_H`
- **Each class**: one `.h` + one `.cpp`, filename matches class name. Some base/utility classes are header-only (no `.cpp`).
- **Comments in Chinese**, Doxygen `/** */` style for public API
- **File header**: `// Created by MeowWow520 on YYYY/M/D`
- **Include paths**: files under `src/` use `core/` prefix (e.g. `#include "core/Game.h"`); files under `src/core/` use relative paths (e.g. `Object/Object.h`, `Config/ConfigManager.h`)
- **Pointer safety**: `SDL_Window*`/`SDL_Renderer*` are raw pointers owned by SDL; `SDL_Texture*` is a raw pointer (`m_texture`), manually destroyed via `SDL_DestroyTexture`

NOTE: `docs/CONTRIBUTING.md` claims `camelCase` variables and `_` suffix for members — the **actual codebase** uses `m_` prefix with snake_case. Follow the actual code, not the doc.

### Def.h key utilities (`src/core/Def.h`)

- `COLOR(0xAABBCCDD)` → expands to `AA, BB, CC, DD` (separated RGBA for `SDL_Color`)
- `EFL_Vec2AddToRect(vec2 pos, vec2 size)` / `EFL_Vec2AddToRectFloat` — glm vec2 to SDL_Rect/SDL_FRect

### Font defaults (`src/core/Font/EFL_FontDef.h`)

- `DEFAULT_FONT_PATH` — `"assets/fonts/Noto_Sans_SC/static/NotoSansSC-ExtraBold.ttf"`
- `DEFAULT_FONT_SIZE` — `200.0F`
- `DEFAULT_TEXT_COLOR` — `{COLOR(0xff4d6dff)}`

### Logger system (`src/core/Logger/`)

The sole logging system. Old `Def.h` macros (`EFL_ClassInit`, `SDL_LibInitChecker`, `CLR_*`) have been removed.

**Log categories** (enum `LogCategory` in `LogCategory.h`): `Core`, `Entity`, `Input`, `Scene`, `Renderer`, `Factory`, `Font`.

**Key macros** (defined in `Log.h`):
- `EFL_LOGGER_INFO(cat, ...)` / `EFL_LOGGER_ERROR(cat, ...)` — categorized log output
- `EFL_LOG_ENTITY_CREATED(name)` / `EFL_LOG_ENTITY_DESTROYED(name)` — called automatically by `Object` constructor/destructor
- `EFL_CHECK(cat, expr, msg)` — if `expr` is falsy, logs error + `return -1`; else logs success. Used in Initialize/Quit for step verification.
- `EFL_CHECK_WITH_GET_ERROR(cat, expr, msg)` — same logic as `EFL_CHECK` but on failure also appends `SDL_GetError()` output to the error log. Prefer this for SDL/TTF/SDL_image API calls; use plain `EFL_CHECK` for non-SDL calls.

**`EFL_CHECK` usage note**: Initialize/Quit return `0` on success, `-1` on failure. So use `!call` or `call == 0` as the check expression:
```cpp
EFL_CHECK(LogCategory::Entity, !entity->Initialize(), "Init");   // 0=success → !0=true → passes
EFL_CHECK(LogCategory::Entity, entity->Quit() == 0, "Quit");     // 0=success → true → passes
```

**Setup**: call `EFL::registerLogCategory()` early in `main()` (before `ConfigManager`/`Game`). `LogConfig` struct accepts custom file paths, truncate/append mode, console/file toggles.

**Quit**: `EFL::quitLogger()` uses individual `spdlog::drop("Core")` etc. — NOT `spdlog::drop_all()` which would also destroy the default logger and crash subsequent `spdlog::info()` calls.

**File sink**: `logs/` directory is auto-created by `std::filesystem::create_directories()` inside `registerLogCategory()`.

**spdlog 1.17.0 caveat**: `sinks_init_list` is `initializer_list<sink_ptr>` — it has **no** `push_back`. When conditionally building sink lists, use `std::vector<spdlog::sink_ptr>` and pass `(name, sinks.begin(), sinks.end())` to the `logger` constructor. The console sink uses `wincolor_stdout_sink_mt` (not `stdout_color_sink_mt`) for Windows console compatibility. Logger name pattern uses `%-7n` (left-aligned), not `%=` (centered).

## Architecture

```
Game (singleton, main loop)
  ├─ KeyboardInput (Input subclass — key→action bindings, movement vector)
  ├─ ConfigManager (singleton — JSON config from assets/json/)
  └─ SceneMain (current scene, created in Game::Initialize)
       ├─ Background (TexturedEntity)
       ├─ Camera (MovableEntity — independent camera, follows player)
       └─ Player (HealthyPointEntity → MovableEntity → TexturedEntity)

Object               ← base: holds Game& ref, m_entity_name, lifecycle methods
  │                    (src/core/Object/Object.h/.cpp)
  ├─ ObjectWorld     ← adds m_world_pos  (src/core/Object/ObjectWorld.h)
  │    └─ ObjectScreen   ← adds m_screen_pos  (src/core/Object/ObjectScreen.h)
  │         ├─ TexturedEntity ← texture, rotation, scale, pivot, blend mode,
  │         │    │               TransScreenPos() (world→screen conversion)
  │         │    ├─ Background
  │         │    ├─ UserInterface
  │         │    └─ MovableEntity ← m_fMaxSpeed, m_fAcceleration, m_vec2_vector
  │         │         ├─ Camera   ← m_vec2_cameraPos, active range, border
  │         │         ├─ HealthyPointEntity ← m_fHealth, m_fGetDamage
  │         │         │    └─ Player   ← m_strPlayerName
  │         │         └─ EnemyRose  (stub — inherits MovableEntity, no methods)
  │         └─ TextBase ← TTF_Text*, TTF_Font*, BaseString
  │              └─ TextStatic  (full lifecycle implementation)
  └─ Scene           ← adds m_vec2_worldSize, m_vec2_worldScale,
                        virtual GetCamera(), virtual GetPlayer()
       ├─ SceneMain  ← main gameplay scene (uses EntityPtr<T> for ownership)
       └─ SceneTitle ← title screen (minimal stub)
```

```
src/core/Logger/          ← logging subsystem
  ├─ LogCategory.h        ← enum: Core, Entity, Input, Scene, Renderer, Factory, Font
  ├─ Log.h                ← EFL_LOGGER_INFO/ERROR macros, LogConfig, registerLogCategory
  └─ Log.cpp              ← registerLogCategory / quitLogger implementation
```

```
src/core/Factory/          ← entity factory
  ├─ Param.h              ← Param<T> template (tagged: Default/Asked/Determined/Custom)
  ├─ EntityTypes.h        ← EntityType enum, param structs (PlayerParams etc.),
  │                          EntityPtr<T> (unique_ptr with custom deleter: Quit() + delete)
  ├─ EntityFactory.h      ← static CreateBackground/Camera/Player methods
  └─ EntityFactory.cpp    ← factory implementations using EFL_CHECK
```

```
Input (standalone — does NOT inherit Object)
  ├─ KeyboardInput  ← m_key_bind, HandleEvents drives press/release,
  │                    GetMovementNormalizeVec2() returns movement direction
  └─ MouseInput     ← stub (empty class, no methods yet)
```

```
Font (singleton manager + entity base class)
  ├─ FontManager (singleton) ← holds TTF_TextEngine*, font lifecycle
  │   src/core/Font/FontManager.h/.cpp
  ├─ EFL_FontDef.h           ← DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE, DEFAULT_TEXT_COLOR
  ├─ TextStypes.h            ← TextStype enum (Static/Colorful), BaseString struct
  └─ TextBase                ← base class (src/core/Font/Base/TextBase.h/.cpp)
  │    │                        inherits ObjectScreen, holds BaseString + TTF_Text* + TTF_Font*
  │    └─ TextStatic          ← full implementation (Initialize/Render/Update/Quit)
  └─ (future: Colorful variant)
```

```
Config (src/core/Config/)
  └─ ConfigManager (singleton) ← loads assets/json/fonts.json + assets/json/default.json
       Structs/
         ├─ BaseJson.h       ← JsonStruct variant (monostate | DefaultJson | FontJson)
         ├─ DefaultStruct.h  ← j_WindowParams, j_PlayerParams, j_CameraParams
         └─ FontStruct.h     ← FontJson (text, stype, font_size, font_path, color, ...)
```

NOTE: `DefaultStruct.h` does not currently contain a `feature` sub-struct, but `MovableEntity::Initialize()` references `m_config_manager.getDefaultJson().feature.acceleration` — the struct is still being developed.

**TransScreenPos()**: defined on `TexturedEntity`, computes screen pos via `m_world_pos - GetCamera()->GetWorldPos()`, reaching camera through `GetCurrentScene()->GetCamera()`.

**SceneMain ownership**: stored as `EntityPtr<T>` (alias for `std::unique_ptr<T, EntityDeleter>`). The custom deleter calls `p->Quit()` then `delete p`. Resetting the pointer in `Quit()` handles cleanup.

**Lifecycle pattern** (all classes follow this):
1. Constructor — initializes members, logs `"XXX object constructed"`
2. `Initialize()` — creates SDL resources, returns 0 on success, -1 on failure
3. `HandleEvents(SDL_Event)` / `Update(float dt)` / `Render()` — per-frame
4. `Quit()` — cleanup, returns 0 on success

Canonical lifecycle return pattern:
```cpp
int ClassName::Initialize() {
    EFL_CHECK(LogCategory::Entity, !SomeInitCall(), "SomeInitCall");
    // ... more setup ...
    return 0;  // success
}
```

**Error handling**: simple `return -1` on failure. No `goto`, no `m_return_code`. `EFL_CHECK` macro handles both logging and early return.

**Known lifecycle traps**:
- **`FontManager::Initialize()` ordering**: MUST be called AFTER `TTF_Init()` AND `SDL_CreateWindowAndRenderer()`. If called before, `GetSDLRenderer()` returns nullptr and `TTF_CreateRendererTextEngine(nullptr)` crashes or fails silently.
- **`FontManager::Initialize()` engine assignment**: MUST assign to `m_text_engine`, NOT a local variable. Correct: `m_text_engine = TTF_CreateRendererTextEngine(...);`. Using `TTF_TextEngine* engine = ...` (local) leaks the engine and makes `GetTTFEngine()` return nullptr.
- `Background::Quit()` must manually `SDL_DestroyTexture(m_texture)` then set `m_texture = nullptr` — `m_texture` is a raw pointer.
- `Camera::Initialize()` does NOT read speed from config. It sets `m_bVisible = false` and computes `m_cameraActiveRange` from world size minus window size. Speed is inherited from MovableEntity's default (`m_fMaxSpeed{750.0f}`).

**Entry point (main.cpp)**:
```
EFL::registerLogCategory()
ConfigManager::getInstance().Initialize()    ← loads JSON from assets/json/
Game::getInstance().Initialize()              ← SDL/TTF/Mix init, FontManager, KeyboardInput, SceneMain
Game::getInstance().Running()                 ← main loop (events → Update → Render → Quit)
EFL::quitLogger()
```

**Game::Running()**: while `m_bRunning`:
- Poll events → dispatch to current scene's HandleEvents
- Call Update with delta time
- Call Render (clears renderer with default clear color, renders scene, presents)
- **Must call `Quit()` at end** (Game::Running() does `Quit(); return 0;` internally) — skipping Quit skips scene destruction, SDL shutdown, and logger flush.

**Coordinate systems**: World position (`m_world_pos`) → screen position (`m_screen_pos = world_pos - camera_pos`). Camera defaults to world center, clamped to active range.

**ConfigManager JSON loading**: Currently reads `assets/json/fonts.json` twice — once for font config and once for default config (the second should probably read `default.json` instead — appears to be a bug).

## How to add a new entity

1. Create `.h`/`.cpp` in `src/core/Entities/` (or `Enemies/` subdirectory for enemies)
2. Inherit from `TexturedEntity`, `MovableEntity`, `HealthyPointEntity`, or `ObjectScreen`/`ObjectWorld` if no texture needed
3. **Constructor must pass `entity_name` up the chain**: each leaf entity constructor invokes its parent with a string literal (e.g. `Player() : HealthyPointEntity("Player") {}`). Use `explicit` on constructors. Intermediate base classes use default parameter forwarding.
4. Register the new `.cpp` in `CMakeLists.txt` under `add_executable`
5. Override lifecycle methods as needed; chain to parent class for default behavior

## How to add a new scene

1. Create `.h`/`.cpp` in `src/` (not under `core/`)
2. Inherit from `Scene`
3. **Constructor must pass `entity_name`** (e.g. `explicit SceneMain() : Scene("SceneMain") {}`)
4. Register in `CMakeLists.txt`
5. Instantiate in `Game::Initialize()` (replace or add scene switching logic)

## CI

Qodana config (`qodana.yaml`) exists but is not wired into CI. No GitHub Actions workflow exists yet.

Manual build-and-run is the primary verification method.

## Git

- `.idea/` and `.opencode/` are in `.gitignore` — IDE settings and OpenCode config are not shared
- Commit convention: Conventional Commits with custom `upd` type (see `docs/CONTRIBUTING.md`)
- Branch naming: `feature/*`, `fix/*`, etc.
