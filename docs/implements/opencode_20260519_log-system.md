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
| 便捷宏 | **`EFL_LOG_INFO(cat, ...)` / `EFL_LOG_ERROR(cat, ...)`** | 替换裸 `spdlog::info()`，一行调用带分类，降低迁移摩擦 |
| EFL_ClassInit 归属 | **归入 `Entity` 类别** | 这两个宏报告的是"实体初始化 / 退出"，语义上属于 entity 生命周期，不算 core |
| 文件输出路径 | **`logs/escape-from-lily.log`** | 位于工作目录，方便 `.gitignore` 忽略 |

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
enum class LogCategory {
    Core,
    Entity,
    Input,
    Scene,
    Render
};
```

辅助函数 `GetCategoryName(LogCategory)` 返回 logger 的实际名称字符串（`"core"`, `"entity"` 等）。

## 新增文件设计

### `src/core/Log.h` — 声明层

```cpp
#pragma once
#include <spdlog/spdlog.h>

namespace efl {   // escape-from-lily

/// 日志分类
enum class LogCategory {
    Core,
    Entity,
    Input,
    Scene,
    Render
};

/// 获取分类名称（即 logger 的注册名）
const char* GetCategoryName(LogCategory cat) noexcept;

/// 注册所有 logger 及其 sink（控制台 + 文件）
void InitLoggers();

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
```

### `src/core/Log.cpp` — 实现层

```cpp
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace efl {
namespace {

/// 创建一个命名 logger，同时挂载控制台 + 文件 sink
void CreateLogger(const char* name) {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%T.%e] [%=7n] [%l]%$ %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/escape-from-lily.log", true   // true = 每次启动截断旧日志
    );
    file_sink->set_pattern("[%Y-%m-%d %T.%e] [%=7n] [%l] %v");

    auto logger = std::make_shared<spdlog::logger>(
        name,
        spdlog::sinks_init_list{console_sink, file_sink}
    );
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

void InitLoggers() {
    CreateLogger("core");
    CreateLogger("entity");
    CreateLogger("input");
    CreateLogger("scene");
    CreateLogger("render");
    spdlog::info("Log system initialized");
}

void ShutdownLoggers() {
    spdlog::drop_all();
}

} // namespace efl
```

**关键注释：**

- 控制台 sink 使用 `%^...%$` 包裹，spdlog 根据级别自动着色（info=白、warn=黄、error=红），不再需要手动拼接 `CLR_RED` 等 ANSI 码
- 文件 sink 不使用颜色码，但保留完整时间戳（包含日期）
- `basic_file_sink_mt` 第二个参数 `true` 表示每次启动截断旧日志
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
    const char* m_entity_name;   // 实体名称，用于日志
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

同理 `MovableEntity`、`Scene` 等中间基类也需要加 `entity_name` 默认参数。

> **注意**：`KeyboardInput` 和 `Scene` 不属于 `Object` 继承链，不受此改动影响。它们的日志直接通过 `EFL_LOG_INFO` 指定 category 输出。

### 构造 / 析构日志输出效果

```
[14:22:35.123] [ entity] [info] Camera object constructed
[14:22:35.140] [ entity] [info] Background object constructed
[14:22:35.156] [ entity] [info] SceneMain object constructed
[14:22:35.170] [ entity] [info] KeyboardInput object constructed
...
[14:23:01.892] [ entity] [info] Background object destroyed
[14:23:01.895] [ entity] [info] KeyboardInput object destroyed
[14:23:01.897] [ entity] [info] Camera object destroyed
```

> 输出格式 `[%=7n]` 保证 `[ entity]` 字段宽度 7，与 `[ core  ]`、`[ input ]`、`[ scene ]`、`[render ]` 对齐。

## 现有代码迁移对照

### `Def.h` 改动

**include 行：** `#include <spdlog/spdlog.h>` → `#include "Log.h"`

**`SDL_LibInitChecker` — 改前：**

```cpp
inline bool SDL_LibInitChecker(const bool flag, std::string name) {
    if (!flag) {
        spdlog::error("{} initialization failed", name);
        return true;
    } spdlog::info("{} initialization successfully", name);
    return false;
}
```

**改后：**

```cpp
inline bool SDL_LibInitChecker(const bool flag, std::string name) {
    if (!flag) {
        EFL_LOG_ERROR(efl::LogCategory::Core, "{} initialization failed", name);
        return true;
    }
    EFL_LOG_INFO(efl::LogCategory::Core, "{} initialization successfully", name);
    return false;
}
```

**`EFL_ClassInit` — 改前：**

```cpp
inline int EFL_ClassInit(const int flag, const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src") + 4;
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        spdlog::error("{}[{}:{}] {}{}{} initialization failed",
            CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
        return flag;
    }
    spdlog::info("{}[{}:{}] {}{}{} initialization successfully",
        CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
    return 0;
}
```

**改后：**

```cpp
inline int EFL_ClassInit(const int flag, const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src") + 4;
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        EFL_LOG_ERROR(efl::LogCategory::Entity, "[{}:{}] {} initialization failed",
            c_file_name, location.line(), c_function_name);
        return flag;
    }
    EFL_LOG_INFO(efl::LogCategory::Entity, "[{}:{}] {} initialization successfully",
        c_file_name, location.line(), c_function_name);
    return 0;
}
```

**`EFL_ClassQuit` — 同理迁移**

### `Game.cpp` 裸 spdlog 调用

**改前：**

```cpp
spdlog::error("{} failed to create window and renderer: {}", m_title.c_str(), SDL_GetError());  // line 48
spdlog::info("{} successfully to create window and renderer", m_title.c_str());                 // line 51
spdlog::info("Receive SDL_EVENT_QUIT events, main loop quitting");                              // line 104
```

**改后：**

```cpp
EFL_LOG_ERROR(efl::LogCategory::Core, "{} failed to create window and renderer: {}", m_title, SDL_GetError());
EFL_LOG_INFO(efl::LogCategory::Core, "{} successfully created window and renderer", m_title);
EFL_LOG_INFO(efl::LogCategory::Core, "SDL_EVENT_QUIT received, main loop quitting");
```

### `Game::Initialize()` — 新增 logger 初始化

在函数开头调用 `efl::InitLoggers()`，最早初始化，确保后续所有日志调用时 logger 已就绪。

### `Game::Quit()` — 新增 logger 注销

```cpp
int Game::Quit() {
    EFL_LOG_INFO(efl::LogCategory::Core, "Game shutting down");
    // ... 其余清理 ...
    efl::ShutdownLoggers();
    return 0;
}
```

`efl::ShutdownLoggers()` 调用 `spdlog::drop_all()` 后，所有 logger 从全局注册表中移除，RAII 析构时会自动 flush 文件 sink。

### `KeyboardInput.cpp` 裸 spdlog 调用

**改前：**

```cpp
if constexpr (SWITCHER_KEYLOGGING)
    spdlog::info("SDL Key_{} Down", SDL_GetKeyName(event.key.key));       // line 26

if constexpr (SWITCHER_KEYLOGGING)
    spdlog::info("SDL Key_{} Up", SDL_GetKeyName(event.key.key));         // line 33
```

**改后：**

```cpp
if constexpr (SWITCHER_KEYLOGGING)
    EFL_LOG_INFO(efl::LogCategory::Input, "Key {} down", SDL_GetKeyName(event.key.key));

if constexpr (SWITCHER_KEYLOGGING)
    EFL_LOG_INFO(efl::LogCategory::Input, "Key {} up", SDL_GetKeyName(event.key.key));
```

## 与 Def.h 现状的关系

当前 `Def.h` 中有两套日志"风格"：

1. **`SDL_LibInitChecker`**：使用裸 `spdlog::info/error`，无分类
2. **`EFL_ClassInit` / `EFL_ClassQuit`**：使用裸 `spdlog::info/error` + ANSI 颜色码拼接

改造后统一改为调用分类宏，颜色由 spdlog pattern 的 `%^...%$` 自动处理。原有的 `CLR_RED` / `CLR_BLUE` / `CLR_YELLOW` 等宏可以从日志中移除调用，但宏定义保留在 `Def.h` 中以备其他用途。

> `SWITCHER_ACCELERATION` / `SWITCHER_KEYLOGGING` / `DEFAULT_MAX_SPEED` 等其他宏不受影响。

## 文件变更清单

| 操作 | 文件 | 说明 |
|------|------|------|
| **新增** | `src/core/Log.h` | 日志基础设施头文件 |
| **新增** | `src/core/Log.cpp` | Logger 注册/注销实现 |
| **新增** | `src/core/Object.cpp` | Object 析构函数实现（原 header-only → 拆分出 cpp） |
| **修改** | `src/core/Object.h` | 构造参数 `entity_name`；析构改为非默认 |
| **修改** | `src/core/Def.h` | `#include` 改为 `Log.h`；`EFL_ClassInit`/`EFL_ClassQuit`/`SDL_LibInitChecker` 内部改用分类宏 |
| **修改** | `src/core/Game.cpp` | 调用 `InitLoggers()`/`ShutdownLoggers()`；裸 spdlog 调用迁移 |
| **修改** | `src/core/Input/KeyboardInput.cpp` | 裸 spdlog 调用迁移 |
| **修改** | `CMakeLists.txt` | 新增 `src/core/Log.cpp`、`src/core/Object.cpp` |
| **修改** | 各实体 `.h`/`.cpp` | 中间基类加 `entity_name` 默认参数；最终实体传入类名实参 |
| **修改** | `.gitignore` | 新增 `logs/` 目录 |

## 构造链 `entity_name` 传递示意图

以 `Background` 为例 —— 从最终实例到基类的完整链路：

```
Background("Background")
  └─ TexturedEntity(game, "Background")    ← 中间基类，原样转发
       └─ ObjectScreen(game, "Background")     ← header-only，转发
            └─ ObjectWorld(game, "Background")     ← header-only，转发
                 └─ Object(game, "Background")        ← 保存到 m_entity_name，打印构造日志
```

每个中间基类只需要在构造函数签名中加 `const char* entity_name = "默认名"`，并转发给父类。`explicit` 关键字保留。

## 实现步骤

1. 创建 `src/core/Log.h` — 枚举、函数声明、便捷宏
2. 创建 `src/core/Log.cpp` — `InitLoggers()` / `ShutdownLoggers()` / `GetCategoryName()` 实现
3. 拆分 `Object.h` → `Object.h` + `Object.cpp`，添加 `entity_name` 参数和析构日志
4. 修改 `Def.h` — include 迁移、内部宏改用分类日志
5. 修改 `Game.cpp` — 集成 `InitLoggers` / `ShutdownLoggers`，迁移裸 spdlog 调用
6. 修改 `KeyboardInput.cpp` — 迁移裸 spdlog 调用
7. 修改各实体 `.cpp` / `.h` — 构造函数传入 entity_name 实参
8. 修改 `CMakeLists.txt` — 注册新文件
9. `.gitignore` 添加 `logs/` 目录
10. 构建验证

## 扩展路线（未来）

- **运行时级别控制**：`GetCategoryName` 返回的 logger 可通过 `spdlog::get("core")->set_level(spdlog::level::warn)` 动态调整输出量
- **日志轮转**：当前用 `basic_file_sink`，后续可替换为 `rotating_file_sink_mt`（按大小轮转）或 `daily_file_sink_mt`（按日期轮转）
- **条件编译**：Release 构建中可将 `EFL_LOG_INFO` 映射到空操作，仅保留 `EFL_LOG_ERROR`
- **结构化日志**：需要时添加 `EFL_LOG_TRACE(cat, ...)` / `EFL_LOG_WARN(cat, ...)` / `EFL_LOG_CRITICAL(cat, ...)` 宏
- **用户自定义类别**：未来如添加 `[audio]` / `[network]` 模块，只需在 `LogCategory` 枚举和 `InitLoggers()` 中各增一行

## 参考

- spdlog 官方文档 — [Custom logger](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers) ; [Sinks](https://github.com/gabime/spdlog/wiki/4.-Sinks)
- 项目现有日志模式：`Def.h` — `SDL_LibInitChecker` / `EFL_ClassInit` / `EFL_ClassQuit`
- 项目实体继承链：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity` → `MovableEntity` → `Camera` / `Player`（见 AGENTS.md）
- 生命周期模式：构造 → `Initialize()` → ... → `Quit()` → 析构（见 `Object.h`）
- 错误处理模式：`goto to_quit` + `EFL_ClassInit` / `EFL_ClassQuit`（见 `Def.h` 和 `SceneMain.cpp`）
- 文件注册惯例：`CMakeLists.txt:22-40` — 所有 `.cpp` 显式注册
- spdlog 依赖方式：vcpkg manifest 提供头文件，header-only，不在 CMake 中 `find_package`（见 AGENTS.md）
