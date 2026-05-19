# 分级日志系统设计方案

> 设计日期：2026/5/19
> 来源：OpenCode 会话规划

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| Logger 架构 | **多命名 logger（5 个）** | spdlog 原生支持 `register_logger` / `get`，按模块隔离输出，语义清晰 |
| Sink 目标 | **控制台 + 文件 双 sink** | 开发期看控制台，调试期查日志文件；spdlog 的 `stdout_color_sink_mt` + `basic_file_sink_mt` 开箱即用 |
| 输出格式 | **spdlog pattern `%^[%T.%e] [%=7n] [%l]%$ %v`** | 替代手动 ANSI 码拼接；`%^...%$` 自动按级别着色；`%=7n` 对齐 logger 名 |
| 实体生命周期 | **方案 A：Object 构造函数接收 entity_name 参数** | 派生类构造时即可获知类名，无需虚函数 / vtable；派生类仅需在构造函数初始值列表中传入一个字面量 |
| 便捷宏 | **`EFL_LOG_INFO(cat, ...)` / `EFL_LOG_ERROR(cat, ...)`** | 一行调用带分类；项目所有新代码统一使用；旧代码的裸 `spdlog::` 调用保留不动 |
| Def.h 处理 | **完全不修改** | `EFL_ClassInit` / `SDL_LibInitChecker` / `CLR_*` 等保持原样，新旧双轨并行，互不干扰 |
| 日志配置 | **`LogConfig` 结构体传入 `InitLoggers`** | 文件路径、截断/追加、控制台/文件开关均可自定义，`{}` 为默认值 |
| 文件输出路径 | **默认 `logs/escape-from-lily.log`，可自定义** | 位于工作目录，方便 `.gitignore` 忽略；调用方可传入任意路径 |

## 分类体系

```
[core]   — Game 主循环、SDL 子系统初始化、窗口/渲染器创建、应用退出
[entity] — Object 构造/析构、各实体 Initialize()/Quit() 成功/失败
[input]  — 键盘事件、按键绑定、mouse/gamepad（未来）
[scene]  — 场景切换、场景初始化/退出
[render] — 纹理加载、渲染调用、设备重置
```

### 分类枚举 (`LogCategory`)

```cpp
/// 日志分类，对应一个命名 logger
enum class LogCategory {
    Core,
    Entity,
    Input,
    Scene,
    Render
};
```

辅助函数 `GetCategoryName(LogCategory)` 返回 logger 的实际名称字符串（`"core"`, `"entity"` 等）。

## 与 Def.h 共存

新 Logger 和旧 Def.h 日志体系**双轨并行**，互不干扰：

- Def.h 中的 `SDL_LibInitChecker` / `EFL_ClassInit` / `EFL_ClassQuit` 继续使用裸 `spdlog::info/error` + ANSI 颜色码（`CLR_BLUE` 等），照常输出到控制台
- `Game.cpp` / `KeyboardInput.cpp` 中的裸 `spdlog::` 调用**保留不动**
- 新 Logger 通过 `spdlog::register_logger` 注册独立的 `spdlog::logger` 实例，拥有自己的 sink（控制台彩色 + 文件），格式与 Def.h 风格不同
- 新代码使用 `EFL_LOG_INFO(cat, ...)` 宏，按分类输出到对应的 named logger

两条线路输出到同一个控制台，但格式独立，泾渭分明：

```
# Def.h 旧风格（保留不变）
[info] [core\Game.cpp:51] Game::Initialize initialization successfully

# 新 Logger 风格（新增）
[14:22:35.123] [ entity] [info] Background object constructed
```

## 新增文件设计

### `src/core/Log.h` — 声明层

```cpp
#pragma once
#ifndef ESCAPE_FROM_LILY_LOG_H
#define ESCAPE_FROM_LILY_LOG_H

#include <spdlog/spdlog.h>
#include <string>

namespace efl {   // escape-from-lily

/// 日志分类
enum class LogCategory {
    Core,
    Entity,
    Input,
    Scene,
    Render
};

/// 日志系统配置
struct LogConfig {
    std::string file_path = "logs/escape-from-lily.log";   ///< 日志文件路径
    bool truncate_file = true;                              ///< true = 每次启动截断旧日志，false = 追加
    bool enable_console = true;                             ///< 是否输出到控制台
    bool enable_file = true;                                ///< 是否输出到文件
};

/// 获取分类名称（即 logger 的注册名）
const char* GetCategoryName(LogCategory cat) noexcept;

/// 注册所有 logger 及其 sink（控制台 + 文件）
void InitLoggers(const LogConfig& config = {});

/// 注销所有 logger、flush 日志
void ShutdownLoggers();

} // namespace efl

/// 便捷宏：按分类输出 info 日志（printf 风格，自动格式检查）
#define EFL_LOG_INFO(cat, ...) \
    spdlog::get(efl::GetCategoryName(cat))->info(__VA_ARGS__)

/// 便捷宏：按分类输出 error 日志
#define EFL_LOG_ERROR(cat, ...) \
    spdlog::get(efl::GetCategoryName(cat))->error(__VA_ARGS__)

/// 便捷宏：实体被构造（在 Object 构造函数中调用）
#define EFL_LOG_ENTITY_CREATED(name) \
    EFL_LOG_INFO(efl::LogCategory::Entity, "{} object constructed", name)

/// 便捷宏：实体被析构（在 Object 析构函数中调用）
#define EFL_LOG_ENTITY_DESTROYED(name) \
    EFL_LOG_INFO(efl::LogCategory::Entity, "{} object destroyed", name)

#endif // ESCAPE_FROM_LILY_LOG_H
```

### `src/core/Log.cpp` — 实现层

```cpp
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace efl {
namespace {

/// 根据配置创建一个命名 logger
void CreateLogger(const char* name, const LogConfig& config) {
    spdlog::sinks_init_list sinks;
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;

    if (config.enable_console) {
        console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%^[%T.%e] [%=7n] [%l]%$ %v");
        sinks.push_back(console_sink);
    }

    if (config.enable_file) {
        file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            config.file_path, config.truncate_file
        );
        file_sink->set_pattern("[%Y-%m-%d %T.%e] [%=7n] [%l] %v");
        sinks.push_back(file_sink);
    }

    auto logger = std::make_shared<spdlog::logger>(name, sinks);
    logger->set_level(spdlog::level::trace);
    spdlog::register_logger(logger);
}

} // anonymous namespace

const char* GetCategoryName(const LogCategory cat) noexcept {
    switch (cat) {
        case LogCategory::Core:   return "core";
        case LogCategory::Entity: return "entity";
        case LogCategory::Input:  return "input";
        case LogCategory::Scene:  return "scene";
        case LogCategory::Render: return "render";
    }
    return "unknown";
}

void InitLoggers(const LogConfig& config) {
    CreateLogger("core",   config);
    CreateLogger("entity", config);
    CreateLogger("input",  config);
    CreateLogger("scene",  config);
    CreateLogger("render", config);
    spdlog::info("Log system initialized, file: {}", config.file_path);
}

void ShutdownLoggers() {
    spdlog::drop_all();
}

} // namespace efl
```

**关键注释：**

- `LogConfig` 支持指定日志文件路径（`file_path`）、截断或追加（`truncate_file`）、控制台和文件独立开关（`enable_console` / `enable_file`），默认 `{}` 即所有开关开启、路径为 `logs/escape-from-lily.log`、截断模式
- 控制台 sink 使用 `%^...%$` 包裹，spdlog 根据级别自动着色（info=白、warn=黄、error=红），不再需要手动拼接 `CLR_RED` 等 ANSI 码
- 文件 sink 不使用颜色码，但保留完整时间戳（包含日期）
- 所有 logger 在 `Game::Initialize()` 中通过 `efl::InitLoggers()` 统一注册，在 `Game::Quit()` 中 `efl::ShutdownLoggers()` 销毁

## Object 改造：构造函数接收实体名称

### 设计思路

C++ 规则：基类构造函数中 `typeid(*this)` 返回**基类**类型，而非派生类。因此无法在 `Object` 构造函数中自动获知派生类名。

解决方案：`Object` 构造函数新增 `const char* entity_name` 参数，派生类在初始值列表中传入自身类名。这是编译期确定的字面量，零运行时开销。

### 改造前（`Object.h`）

```cpp
/// 万物之源
class Object {
public:
    explicit Object(Game& game_instance) noexcept;
    virtual ~Object() = default;

    virtual void HandleEvents(SDL_Event event);
    virtual void Update(float dt);
    virtual void Render();
    virtual int Initialize();
    virtual int Quit();

protected:
    Game& m_game_instance;
    int m_return_code{};
};
```

### 改造后（`Object.h` + 新增 `Object.cpp`）

**`Object.h`：**

```cpp
/// 万物之源
class Object {
public:
    explicit Object(Game& game_instance, const char* entity_name = "Object") noexcept;
    virtual ~Object();   // 改为非默认，在 .cpp 中实现析构日志

    virtual void HandleEvents(SDL_Event event);
    virtual void Update(float dt);
    virtual void Render();
    virtual int Initialize();
    virtual int Quit();

protected:
    Game& m_game_instance;
    const char* m_entity_name;   ///< 实体名称，用于日志
    int m_return_code{};
};
```

**`Object.cpp`（新增）：**

```cpp
#include "Object.h"
#include "Log.h"

Object::Object(Game& game_instance, const char* entity_name) noexcept
    : m_game_instance(game_instance), m_entity_name(entity_name) {
    EFL_LOG_ENTITY_CREATED(m_entity_name);
}

Object::~Object() {
    EFL_LOG_ENTITY_DESTROYED(m_entity_name);
}
```

析构函数改为非默认的原因：需要在析构函数中调用 `EFL_LOG_ENTITY_DESTROYED(m_entity_name)`。`Object.h` 原来是 header-only，改动后需新增 `Object.cpp` 并在 `CMakeLists.txt` 中注册。

### 派生类改造示例

每个派生类在构造函数的初始值列表中传入类名。以 `Background` 为例：

**改造前（`Background.cpp`）：**

```cpp
Background::Background(Game& game_instance) noexcept
    : TexturedEntity(game_instance) {}
```

**改造后（`Background.cpp`）：**

```cpp
Background::Background(Game& game_instance) noexcept
    : TexturedEntity(game_instance, "Background") {}
```

中间基类带默认值，保证不传参时也能编译：

**`TexturedEntity.h` 改前：**

```cpp
explicit TexturedEntity(Game& game_instance) noexcept;
```

**`TexturedEntity.h` 改后：**

```cpp
explicit TexturedEntity(Game& game_instance, const char* entity_name = "TexturedEntity") noexcept;
```

同理 `MovableEntity`、`Scene`、`ObjectWorld`、`ObjectScreen` 等中间基类也需要加 `entity_name` 默认参数并转发给父类。

> **注意**：`KeyboardInput` 和 `Input` 不继承 `Object`，不受此改动影响。

### 构造 / 析构日志输出效果

```
[14:22:35.123] [ entity] [info] Camera object constructed
[14:22:35.140] [ entity] [info] Background object constructed
[14:22:35.156] [ entity] [info] SceneMain object constructed
...
[14:23:01.892] [ entity] [info] Background object destroyed
[14:23:01.895] [ entity] [info] Camera object destroyed
[14:23:01.897] [ entity] [info] SceneMain object destroyed
```

> 输出格式 `[%=7n]` 保证 `[ entity]` 字段宽度 7，与 `[ core  ]`、`[ input ]`、`[ scene ]`、`[render ]` 对齐。

## 构造链 `entity_name` 传递示意图

以 `Background` 为例 —— 从最终实例到基类的完整链路：

```
Background("Background")
  └─ TexturedEntity(game_instance, "Background")   ← 中间基类，原样转发
       └─ ObjectScreen(game_instance, "Background")    ← header-only，转发
            └─ ObjectWorld(game_instance, "Background")    ← header-only，转发
                 └─ Object(game_instance, "Background")        ← 保存到 m_entity_name，打印构造日志
```

每个中间基类只需要在构造函数签名中加 `const char* entity_name = "默认名"`，并转发给父类。`explicit` 关键字保留。

## Game 集成

### `Game.cpp` — 新增 logger 初始化与注销

仅新增 2 行调用，其余代码**完全不动**：

```cpp
int Game::Initialize() {
    efl::InitLoggers();   // ← 新增：最早初始化，确保后续所有日志调用时 logger 已就绪

    if (SDL_LibInitChecker(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), "SDL_Init")) {
        m_return_code = -1;
        goto to_quit;
    }
    // ... 其余初始化完全不动 ...
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}

int Game::Quit() {
    EFL_LOG_INFO(efl::LogCategory::Core, "Game shutting down");  // ← 新增
    // ... 其余清理完全不动 ...
    efl::ShutdownLoggers();  // ← 新增：flush 并注销所有 logger
    return 0;
}
```

> `efl::InitLoggers()` 不传参时使用默认 `LogConfig{}`（路径 `logs/escape-from-lily.log`，截断模式）。如需自定义，改为 `efl::InitLoggers({.file_path = "logs/my.log", .truncate_file = false})`。

## 文件变更清单

| 操作 | 文件 | 说明 |
|------|------|------|
| **新增** | `src/core/Log.h` | `LogConfig` + `LogCategory` + 宏 + 函数声明 |
| **新增** | `src/core/Log.cpp` | `InitLoggers(config)` / `ShutdownLoggers()` 实现 |
| **新增** | `src/core/Object.cpp` | Object 构造/析构函数实现（从 header-only 拆出） |
| **修改** | `src/core/Object.h` | 构造参数加 `entity_name`；析构改为非默认；新增 `m_entity_name` 成员 |
| **修改** | `src/core/ObjectWorld.h` | 构造函数加 `entity_name` 默认参数并转发给 Object |
| **修改** | `src/core/ObjectScreen.h` | 同上 |
| **修改** | `src/core/Entities/Base/TexturedEntity.h` | 同上 |
| **修改** | `src/core/Entities/Base/MovableEntity.h` | 同上 |
| **修改** | `src/core/Scene.h` | 同上（Scene 继承 Object） |
| **修改** | `src/core/Game.cpp` | 新增 2 行：`InitLoggers()` + `ShutdownLoggers()` + 1 行 quit 日志 |
| **修改** | `src/core/Entities/Background.cpp` | 构造传 `"Background"` |
| **修改** | `src/core/Entities/Camera.cpp` | 构造传 `"Camera"` |
| **修改** | `src/core/Entities/Player.cpp` | 构造传 `"Player"` |
| **修改** | `src/core/Entities/UserInterface.cpp` | 构造传 `"UserInterface"` |
| **修改** | `src/SceneMain.cpp` | 构造传 `"SceneMain"` |
| **修改** | `src/SceneTitle.cpp` | 构造传 `"SceneTitle"` |
| **修改** | `CMakeLists.txt` | 新增 `src/core/Log.cpp`、`src/core/Object.cpp` |
| **修改** | `.gitignore` | 新增 `logs/` 目录 |
| **不动** | `src/core/Def.h` | 零改动 |
| **不动** | `src/core/Input/KeyboardInput.cpp` | 零改动 |
| **不动** | `src/core/Input/Input.cpp` | 零改动 |

## 实现步骤

1. 创建 `src/core/Log.h` — `LogConfig`、`LogCategory` 枚举、便捷宏、函数声明
2. 创建 `src/core/Log.cpp` — `InitLoggers(config)` / `ShutdownLoggers()` / `GetCategoryName()` 实现
3. 拆分 `Object.h` → `Object.h` + `Object.cpp`，添加 `entity_name` 参数和构造/析构日志
4. 修改 `Game.cpp` — `Initialize()` 开头加 `efl::InitLoggers()`；`Quit()` 末尾加 `efl::ShutdownLoggers()`
5. 修改各中间基类 `.h` — 构造函数加 `entity_name` 默认参数并转发父类
6. 修改各叶子实体 `.cpp` — 构造函数传入具体实体名
7. 修改 `CMakeLists.txt` — 注册 `Log.cpp` + `Object.cpp`
8. `.gitignore` 添加 `logs/` 目录
9. 构建验证

## 扩展路线（未来）

- **运行时级别控制**：通过 `spdlog::get("core")->set_level(spdlog::level::warn)` 动态调整各分类输出量
- **日志轮转**：当前用 `basic_file_sink`，后续可替换为 `rotating_file_sink_mt`（按大小轮转，如 10MB × 3 文件）或 `daily_file_sink_mt`（按日期轮转）
- **条件编译**：Release 构建中可将 `EFL_LOG_INFO` 宏映射到空操作，仅保留 `EFL_LOG_ERROR`
- **结构化日志**：需要时添加 `EFL_LOG_TRACE(cat, ...)` / `EFL_LOG_WARN(cat, ...)` / `EFL_LOG_CRITICAL(cat, ...)` 宏
- **用户自定义类别**：未来如添加 `[audio]` / `[network]` 模块，只需在 `LogCategory` 枚举和 `InitLoggers()` 中各增一行
- **多文件输出**：可在 `LogConfig` 中扩展为按分类指定不同文件路径，如 core 写 `core.log`、entity 写 `entity.log`

## 参考

- spdlog 官方文档 — [Custom logger](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers) ; [Sinks](https://github.com/gabime/spdlog/wiki/4.-Sinks)
- 项目现有日志模式：`Def.h` — `SDL_LibInitChecker` / `EFL_ClassInit` / `EFL_ClassQuit`
- 项目实体继承链：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity` → `MovableEntity` → `Camera` / `Player`（见 AGENTS.md）
- 生命周期模式：构造 → `Initialize()` → ... → `Quit()` → 析构（见 `Object.h`）
- 错误处理模式：`goto to_quit` + `EFL_ClassInit` / `EFL_ClassQuit`（见 `Def.h` 和 `SceneMain.cpp`）
- 文件注册惯例：`CMakeLists.txt:22-40` — 所有 `.cpp` 显式注册
- spdlog 依赖方式：vcpkg manifest 提供头文件，header-only，不在 CMake 中 `find_package`（见 AGENTS.md）
