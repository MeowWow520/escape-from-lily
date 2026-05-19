# 分级日志系统设计方案

> 设计日期：2026/5/19
> 来源：OpenCode 会话规划

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| Logger 架构 | **多命名 logger（5 个）** | spdlog 原生支持 `register_logger` / `get`，按模块隔离输出，语义清晰 |
| Sink 目标 | **控制台 + 文件 双 sink** | 开发期看控制台，调试期查日志文件；`wincolor_stdout_sink_mt` + `basic_file_sink_mt` 双路并行 |
| 输出格式 | **spdlog pattern `%^[%T.%e] [%=7n] [%l]%$ %v`** | `%^...%$` 自动按级别着色；`%=7n` 对齐 logger 名；与 Def.h 的 ANSI 码风格独立 |
| 实体生命周期 | **方案 A：Object 构造函数接收 entity_name 参数** | 派生类构造时即可获知类名，无需虚函数 / vtable；叶子类仅需在构造初始值列表中传入一个字面量 |
| Def.h 处理 | **完全不修改** | `EFL_ClassInit` / `SDL_LibInitChecker` / `CLR_*` 等保持原样，新旧双轨并行，互不干扰 |
| 日志配置 | **`LogConfig` 结构体传入 `RegisterLogCategory`** | 文件路径、截断/追加、控制台/文件开关均可自定义；`{}` 为默认值 |

## 分类体系

```
[Core]     — Game 主循环、SDL 子系统初始化、窗口/渲染器创建、应用退出
[Entity]   — Object 构造/析构、各实体 Initialize()/Quit() 成功/失败
[Input]    — 键盘事件、按键绑定、mouse/gamepad（未来）
[Scene]    — 场景切换、场景初始化/退出
[Renderer] — 纹理加载、渲染调用、设备重置
```

### 分类枚举

枚举定义在 `src/core/Logger/LogCategory.h`（独立文件，供 Log.h 包含）：

```cpp
enum class LogCategory {
    Core,     // 游戏核心
    Entity,   // 实体生命周期
    Input,    // 输入事件
    Scene,    // 场景管理
    Renderer  // 渲染相关
};
```

## 与 Def.h 共存

新 Logger 和旧 Def.h 日志体系**双轨并行**，互不干扰：

- Def.h 中的 `SDL_LibInitChecker` / `EFL_ClassInit` / `EFL_ClassQuit` 继续使用裸 `spdlog::info/error` + ANSI 颜色码（`CLR_BLUE` 等），照常输出到控制台
- `Game.cpp` / `KeyboardInput.cpp` 中的裸 `spdlog::` 调用**保留不动**
- 新 Logger 通过 `spdlog::register_logger` 注册独立的 `spdlog::logger` 实例，拥有自己的 sink（`wincolor_stdout_sink_mt` + `basic_file_sink_mt`），格式与 Def.h 风格不同
- 新代码使用 `EFL_LOGGER_INFO(cat, ...)` 宏，按分类输出到对应的 named logger

两条线路输出到同一个控制台，格式独立：

```
# Def.h 旧风格（保留不变）
[info] [core\Game.cpp:51] Game::Initialize initialization successfully

# 新 Logger 风格（新增）
[14:22:35.123] [ Entity] [info] Background object constructed
```

> 注意：logger 注册名使用首字母大写（`"Core"`、`"Entity"` 等），`%=7n` 格式化后字段对齐。

## 文件设计

目前已实现的文件结构：

```
src/core/Logger/
├── LogCategory.h    ← 分类枚举
├── Log.h            ← LogConfig 结构体、宏定义、函数声明
└── Log.cpp          ← RegisterLogCategory / QuitLogger 实现
```

### `LogCategory.h`

```cpp
// Created by MeowWow520 on 2026/5/19.

#ifndef ESCAPE_FROM_LILY_LOGCATEGORY_H
#define ESCAPE_FROM_LILY_LOGCATEGORY_H

enum class LogCategory {
    Core,     // 游戏核心 — SDL 子系统、窗口渲染、应用退出
    Entity,   // 实体生命周期 — Object 构造/析构、Initialize/Quit
    Input,    // 输入事件 — 键盘/鼠标/手柄
    Scene,    // 场景管理 — 场景切换、状态变化
    Renderer  // 渲染相关 — 纹理加载、渲染调用、设备重置
};

#endif
```

### `Log.h`

```cpp
#pragma once

#ifndef ESCAPE_FROM_LILY_LOG_H
#define ESCAPE_FROM_LILY_LOG_H
#include <spdlog/spdlog.h>
#include <string>
#include "LogCategory.h"

namespace EFL {

/// 日志系统配置
struct LogConfig {
    std::string file_path = "logs/escape-from-lily.log";
    bool truncate_log_file = true;    ///< true = 每次启动截断；false = 追加
    bool console_logging = true;      ///< 是否输出到控制台
    bool enable_logging = true;       ///< 是否输出到文件
};

/// 获取分类名称（即 logger 的注册名，首字母大写）
const char* GetCategoryName(LogCategory cate) noexcept;

/// 注册所有 logger 及其 sink（控制台 + 文件）
void RegisterLogCategory(const LogConfig& config = {});

/// 注销所有 logger、flush 日志
void QuitLogger();

} // namespace EFL

/// 便捷宏：按分类输出 info 日志
#define EFL_LOGGER_INFO(cat, ...) \
    spdlog::get(EFL::GetCategoryName(cat))->info(__VA_ARGS__)

/// 便捷宏：按分类输出 error 日志
#define EFL_LOGGER_ERROR(cat, ...) \
    spdlog::get(EFL::GetCategoryName(cat))->error(__VA_ARGS__)

/// 便捷宏：实体被构造（在 Object 构造函数中调用）
#define EFL_LOG_ENTITY_CREATED(name) \
    EFL_LOGGER_INFO(LogCategory::Entity, "{} object constructed", name)

/// 便捷宏：实体被析构（在 Object 析构函数中调用）
#define EFL_LOG_ENTITY_DESTROYED(name) \
    EFL_LOGGER_INFO(LogCategory::Entity, "{} object destroyed", name)

#endif
```

### `Log.cpp`

```cpp
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace EFL {
namespace {

/// 根据配置创建一个命名 logger
void CreateLogger(const char* name, const LogConfig& config) {
    std::vector<spdlog::sink_ptr> sinks;   // sink_ptr = shared_ptr<sinks::sink>

    if (config.console_logging) {
        const auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
        console_sink->set_pattern("%^[%T.%e] [%=7n] [%l]%$ %v");
        sinks.push_back(console_sink);
    }

    if (config.enable_logging) {
        const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            config.file_path, config.truncate_log_file);
        file_sink->set_pattern("[%Y-%m-%d %T.%e] [%=7n] [%l] %v");
        sinks.push_back(file_sink);
    }

    // spdlog 1.17.0 中 sinks_init_list 是 initializer_list，无 push_back
    // 改用 vector + 迭代器构造
    auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    spdlog::register_logger(logger);
}

} // anonymous namespace

const char* GetCategoryName(const LogCategory cate) noexcept {
    switch (cate) {
        case LogCategory::Core:     return "Core";
        case LogCategory::Entity:   return "Entity";
        case LogCategory::Input:    return "Input";
        case LogCategory::Scene:    return "Scene";
        case LogCategory::Renderer: return "Renderer";
    }
    return "unknown";
}

void RegisterLogCategory(const LogConfig& config) {
    CreateLogger("Core",     config);
    CreateLogger("Entity",   config);
    CreateLogger("Input",    config);
    CreateLogger("Scene",    config);
    CreateLogger("Renderer", config);
    spdlog::info("Log system initialized, file: {}", config.file_path);
}

void QuitLogger() {
    spdlog::drop_all();
}

} // namespace EFL
```

**关键注释：**

- 本项目使用 **spdlog 1.17.0**，其中 `sinks_init_list` 是 `std::initializer_list<sink_ptr>`，**没有** `push_back` 方法。条件性添加 sink 时改用 `std::vector<spdlog::sink_ptr>`，再通过 logger 的迭代器构造函数 `(name, begin, end)` 创建
- 控制台 sink 使用 `wincolor_stdout_sink_mt`（Windows Console API 着色），而非 `stdout_color_sink_mt`（ANSI 转义序列）；后者在传统 cmd.exe 下可能不生效
- `GetCategoryName` 返回首字母大写的 logger 名（`"Core"`），与 pattern 的 `[%=7n]` 配合对齐输出

### 自定义日志文件路径

`RegisterLogCategory` 接受 `LogConfig` 参数：

```cpp
// 默认路径 logs/escape-from-lily.log，截断模式
EFL::RegisterLogCategory();

// 自定义路径，追加模式
EFL::RegisterLogCategory({
    .file_path = "logs/debug.log",
    .truncate_log_file = false
});

// 仅控制台，不写文件
EFL::RegisterLogCategory({.enable_logging = false});
```

在 `Game::Initialize()` 开头调用即可。

## Object 改造：构造函数接收实体名称

### 设计思路

C++ 规则：基类构造函数中 `typeid(*this)` 返回**基类**类型，而非派生类。因此无法在 `Object` 构造函数中自动获知派生类名。

解决方案：`Object` 构造函数接收 `const char* entity_name` 参数，派生类在初始值列表中传入自身类名。这是编译期确定的字面量，零运行时开销。

### Object.h 当前状态

```cpp
class Object {
protected:
    Game& m_game_instance = Game::GetInstance();
    const char* m_entity_name;       ///< 实体名称，用于日志
    int m_return_code;

public:
    /// @param m_game_instance  Game 单例引用
    /// @param m_entity_name    实体名（如 "Background"），用于构造/析构日志
    /// @param m_return_code    返回值占位（由 Initialize/Quit 回填，不在此使用）
    Object(Game& m_game_instance, const char* m_entity_name, int m_return_code) noexcept;
    virtual ~Object();

    virtual int Initialize() { return 0; }
    virtual void HandleEvents(SDL_Event event) { }
    virtual void Update(float dt) { }
    virtual void Render() { }
    virtual int Quit() { return 0; }
};
```

### Object.cpp 当前状态

```cpp
#include "Object.h"
#include "../Logger/Log.h"

Object::Object(Game &m_game_instance, const char *m_entity_name, int m_return_code) noexcept
    : m_game_instance(m_game_instance),
      m_entity_name(m_entity_name),
      m_return_code(0)          // 第三个参数只是占位，实际固定初始化为 0
{
    EFL_LOG_ENTITY_CREATED(m_entity_name);
}

Object::~Object() {
    EFL_LOG_ENTITY_DESTROYED(m_entity_name);
}
```

### 为什么派生类全部需要修改

`Object` 不再有默认构造函数（3 个参数且无默认值）。因此**所有继承链上的类**必须显式调用带参的 `Object` 构造函数。使用 `= default` 或无参构造函数的类会编译失败。

方案：中间基类新增带 `entity_name` 默认参数的**显式构造函数**，转发给父类；叶子类在初始值列表中传入类名字面量。

## 各派生类改动对照

### 中间基类

改动模式统一：**从 `= default` / 无参构造 → 改为显式构造函数**，加 `entity_name` 参数并转发。

**`ObjectWorld.h` — 改前：**

```cpp
class ObjectWorld : public Object {
protected:
    glm::vec2 m_world_pos{};
public:
    ObjectWorld() = default;          // ← 调用 Object()，编译错误！
    ~ObjectWorld() override = default;
    // ...
};
```

**改后：**

```cpp
class ObjectWorld : public Object {
protected:
    glm::vec2 m_world_pos{};
public:
    explicit ObjectWorld(const char* entity_name = "ObjectWorld")
        : Object(Game::GetInstance(), entity_name, 0) {}
    ~ObjectWorld() override = default;
    // ...
};
```

**`ObjectScreen.h`、`TexturedEntity.h`、`MovableEntity.h`、`Scene.h` — 同理：**

```cpp
// ObjectScreen.h
explicit ObjectScreen(const char* entity_name = "ObjectScreen")
    : ObjectWorld(entity_name) {}

// TexturedEntity.h
explicit TexturedEntity(const char* entity_name = "TexturedEntity")
    : ObjectScreen(entity_name) {}

// MovableEntity.h
explicit MovableEntity(const char* entity_name = "MovableEntity")
    : TexturedEntity(entity_name) {}

// Scene.h
explicit Scene(const char* entity_name = "Scene")
    : Object(Game::GetInstance(), entity_name, 0) {}
```

> 注意：之前 `TexturedEntity` 和 `MovableEntity` 声明的是 `TexturedEntity();` / `MovableEntity();`（无参声明），现在需替换为带默认参数的显式构造函数。

### 叶子实体

改动模式统一：**如果原来没有构造函数声明**（编译器隐式生成默认构造），则需新增一行显式构造函数；**如果原来有 `= default`**，则替换为带参版本。初始值列表传入实体名。

| 类 | 当前状态 | 改后 |
|---|---|---|
| `Background` | 无显式构造函数（隐式默认构造调用 `TexturedEntity()`） | `Background() : TexturedEntity("Background") {}` |
| `Camera` | 同上 | `Camera() : MovableEntity("Camera") {}` |
| `Player` | 同上 | `Player() : MovableEntity("Player") {}` |
| `SceneMain` | `SceneMain() = default;` | `SceneMain() : Scene("SceneMain") {}` |
| `SceneTitle` | 无显式构造函数（隐式默认，继承 `Scene`） | `SceneTitle() : Scene("SceneTitle") {}` |
| `UserInterface` | 无显式构造函数 | `UserInterface() : TexturedEntity("UserInterface") {}` |

**示例：`Background.h` 当前 → 改后：**

```cpp
// 改前：无构造函数声明，编译器生成隐式默认构造函数
class Background : public TexturedEntity {
public:
    int Initialize() override;
    void Update(float dt) override;
    void Render() override;
    int Quit() override;
};
```

```cpp
// 改后：新增一行显式构造函数
class Background : public TexturedEntity {
public:
    Background() : TexturedEntity("Background") {}    // ← 新增
    int Initialize() override;
    void Update(float dt) override;
    void Render() override;
    int Quit() override;
};
```

**`SceneMain.h` 当前 → 改后：**

```cpp
// 改前
class SceneMain : public Scene {
public:
    SceneMain() = default;    // ← 隐式调用 Scene()，Object 没有 Scene()！
    // ...
};
```

```cpp
// 改后
class SceneMain : public Scene {
public:
    SceneMain() : Scene("SceneMain") {}    // ← 显式传名
    // ...
};
```

### 外部调用方不变

所有使用 `new` 的调用方一行不改：

```cpp
// SceneMain.cpp — 无需任何修改
m_camera = new Camera();
m_current_background = new Background();
// Camera() 和 Background() 仍然是无参调用，entity_name 在构造链内部传递
```

### 构造链传递示意

以 `Background` 为例，从叶子到基类的完整链路：

```
Background()                                    ← 无参，调用 TexturedEntity("Background")
  └─ TexturedEntity(entity_name)                ← 默认 "TexturedEntity"，但叶子传 "Background"
       └─ ObjectScreen(entity_name)             ← 默认 "ObjectScreen"
            └─ ObjectWorld(entity_name)         ← 默认 "ObjectWorld"
                 └─ Object(inst, entity_name, 0) ← 到达基类，打印 "Background object constructed"
```

中间基类有默认值 `"TexturedEntity"` 等，确保直接构造中间类也不会出错（但实际不会直接实例化）。

## Game 集成

`Game::Initialize()` 开头调用 `EFL::RegisterLogCategory()`，`Game::Quit()` 末尾调用 `EFL::QuitLogger()`：

```cpp
int Game::Initialize() {
    EFL::RegisterLogCategory();   // ← 最早，确保后续代码可用新 logger

    // ... 其余初始化完全不动 ...
}

int Game::Quit() {
    EFL_LOGGER_INFO(LogCategory::Core, "Game shutting down");
    // ... 其余清理 ...
    EFL::QuitLogger();
    return 0;
}
```

## 日志输出效果

```
# 构造阶段
[14:22:35.123] [ Entity] [info] Camera object constructed
[14:22:35.140] [ Entity] [info] Background object constructed
[14:22:35.156] [ Entity] [info] SceneMain object constructed

# 运行阶段
[14:22:35.170] [   Core] [info] Log system initialized, file: logs/escape-from-lily.log
[14:22:35.180] [   Core] [info] escape-from-lily successfully created window and renderer

# 输入事件（未来）
[14:22:36.500] [  Input] [info] Key W down

# 析构阶段
[14:23:01.892] [ Entity] [info] Background object destroyed
[14:23:01.895] [ Entity] [info] Camera object destroyed
[14:23:01.900] [   Core] [info] Game shutting down
```

> `[%=7n]` 保证 5 个分类名对齐：`[   Core]`、`[ Entity]`、`[  Input]`、`[  Scene]`、`[Renderer]`。

## 文件变更清单

| 操作 | 文件 | 说明 |
|------|------|------|
| ✅ 已创建 | `src/core/Logger/LogCategory.h` | 分类枚举 |
| ✅ 已创建 | `src/core/Logger/Log.h` | LogConfig + 宏 + 函数声明 |
| ✅ 已创建 | `src/core/Logger/Log.cpp` | RegisterLogCategory / QuitLogger 实现 |
| ✅ 已修改 | `src/core/Object/Object.h` | 构造参数 3 个，析构非默认，新增 `m_entity_name` |
| ✅ 已修改 | `src/core/Object/Object.cpp` | 构造/析构中调用 EFL_LOG_ENTITY_CREATED/DESTROYED |
| ✅ 已修改 | `CMakeLists.txt` | 注册 `Logger/*` 和 `Object.cpp` |
| **待修改** | `src/core/Object/ObjectWorld.h` | `= default` → 显式构造函数，转发 `entity_name` |
| **待修改** | `src/core/Object/ObjectScreen.h` | 同上 |
| **待修改** | `src/core/Entities/Base/TexturedEntity.h` | `TexturedEntity();` → 带默认参构造函数，转发 |
| **待修改** | `src/core/Entities/Base/MovableEntity.h` | `MovableEntity();` → 带默认参构造函数，转发 |
| **待修改** | `src/core/Scene.h` | `Scene() = default;` → 显式构造函数，转发 |
| **待修改** | `src/core/Entities/Background.h` | 新增 `Background() : TexturedEntity("Background") {}` |
| **待修改** | `src/core/Entities/Camera.h` | 新增 `Camera() : MovableEntity("Camera") {}` |
| **待修改** | `src/core/Entities/Player.h` | 新增 `Player() : MovableEntity("Player") {}` |
| **待修改** | `src/core/Entities/UserInterface.h` | 新增 `UserInterface() : TexturedEntity("UserInterface") {}` |
| **待修改** | `src/SceneMain.h` | `SceneMain() = default;` → `SceneMain() : Scene("SceneMain") {}` |
| **待修改** | `src/SceneTitle.h` | 新增 `SceneTitle() : Scene("SceneTitle") {}` |
| **待修改** | `src/core/Game.cpp` | 新增 `RegisterLogCategory()` / `QuitLogger()` 两行 |
| **待修改** | `.gitignore` | 新增 `logs/` |
| 不动 | `src/core/Def.h` | 零改动 |
| 不动 | `src/core/Input/` | 零改动 |
| 不动 | `src/SceneMain.cpp` | 零改动（只是 new 调用方） |

## 实现步骤

1. ~~创建 `src/core/Logger/` 目录及 `LogCategory.h`、`Log.h`、`Log.cpp`~~ ✅ 已完成
2. ~~改造 `Object.h` + `Object.cpp`~~ ✅ 已完成
3. ~~更新 `CMakeLists.txt`~~ ✅ 已完成
4. 修改 `Game.cpp` — `Initialize()` 开头加 `EFL::RegisterLogCategory()`；`Quit()` 加 `EFL::QuitLogger()`
5. 修改 5 个中间基类（`ObjectWorld.h`、`ObjectScreen.h`、`TexturedEntity.h`、`MovableEntity.h`、`Scene.h`）— 构造转发 `entity_name`
6. 修改 6 个叶子实体/场景（`Background.h`、`Camera.h`、`Player.h`、`UserInterface.h`、`SceneMain.h`、`SceneTitle.h`）— 构造传入实体名
7. `.gitignore` 添加 `logs/` 目录
8. 构建验证

## 扩展路线（未来）

- **运行时级别控制**：通过 `spdlog::get("Core")->set_level(spdlog::level::warn)` 动态调整各分类输出量
- **日志轮转**：当前用 `basic_file_sink`，后续可替换为 `rotating_file_sink_mt`（按大小轮转）或 `daily_file_sink_mt`（按日期轮转）
- **条件编译**：Release 构建中可将 `EFL_LOGGER_INFO` 宏映射到空操作，仅保留 `EFL_LOGGER_ERROR`
- **更多级别**：需要时添加 `EFL_LOGGER_TRACE/WARN/CRITICAL(cat, ...)` 宏
- **用户自定义类别**：如添加 `[Audio]` / `[Network]` 模块，在 `LogCategory` 枚举和 `RegisterLogCategory()` 中各增一行
- **多文件输出**：可在 `LogConfig` 中扩展为按分类指定不同文件路径

## 参考

- spdlog 1.17.0 官方文档 — [Custom logger](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers) ; [Sinks](https://github.com/gabime/spdlog/wiki/4.-Sinks)
- 项目现有日志模式：`Def.h` — `SDL_LibInitChecker` / `EFL_ClassInit` / `EFL_ClassQuit`
- 项目实体继承链：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity` → `MovableEntity` → `Camera` / `Player`（见 AGENTS.md）
- 生命周期模式：构造 → `Initialize()` → ... → `Quit()` → 析构
- 错误处理模式：`goto to_quit` + `EFL_ClassInit` / `EFL_ClassQuit`
- spdlog 依赖方式：vcpkg manifest 提供头文件，header-only，不在 CMake 中 `find_package`
