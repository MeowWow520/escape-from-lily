# 简单工厂（Simple Factory）设计方案

> 设计日期：2026/5/19
> 来源：OpenCode 会话规划

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| 工厂模式 | **简单工厂（参数化创建）** | 实体类型有限（~4 种），无需 GoF Factory Method 的 Creator 继承体系 |
| 创建流程 | **两阶段：构造 → Initialize** | 遵循项目现有生命周期惯例（见 AGENTS.md / Object.h） |
| 错误处理 | **返回 nullptr + 清理已分配资源** | 与 Initialize() 返回 0/非0 一致；调用方检查 nullptr |
| 参数传递 | **按类型定义参数结构体** | 每个实体参数不同（Background 需要 path，Player 需要 name），结构体比零散参数更清晰 |
| 所有权 | **返回 raw pointer，调用方负责 delete** | 与项目现有 new/delete 模式一致（见 SceneMain.cpp） |

## 模式概述

简单工厂将"对象创建 + 初始化"封装到一个类中，对外暴露统一的 `Create()` 接口，内部根据传入的实体类型分发到对应的构造逻辑。

```
调用方（Scene）
  │
  ▼
EntityFactory::Create(EntityType, Params)
  │
  ├─ case Player:    → new Player()   → SetName() → Initialize()
  ├─ case Background → new Background() → SetPath() → Initialize()
  ├─ case Camera:    → new Camera()   → Initialize()
  └─ case UI:        → new UserInterface() → Initialize()
```

对比当前 `SceneMain::Initialize()` 中的做法（`new` → 手动调 setter → `Initialize` → 检查返回值 → `goto to_quit`），工厂方法将这一段重复模板内聚，减少出错和代码冗余。

## 实体类型枚举

```cpp
/// 实体类型标识，用于工厂分发
enum class EntityType : uint8_t {
    Player,
    Background,
    Camera,
    UserInterface
    // 未来扩展：Enemy, NPC, Item ...
};
```

## 参数结构体

每种实体有自己的参数结构体。工厂通过 `std::variant` 或 `void*` 多态传参——本设计推荐 `std::variant`，类型安全。

```cpp
/// Player 创建参数
struct PlayerParams {
    std::string name;           // 玩家名称
    glm::vec2 world_pos{};      // 初始世界坐标
};

/// Background 创建参数
struct BackgroundParams {
    std::string texture_path;   // 纹理文件路径
    glm::vec2 world_pos{};      // 初始世界坐标（通常为 {0, 0}）
};

/// Camera 创建参数
struct CameraParams {
    glm::vec2 world_pos{};      // 初始相机位置
};

/// UserInterface 创建参数
struct UIParams {
    std::string texture_path;
    glm::vec2 screen_pos{};     // UI 使用屏幕坐标
};

/// 统一的参数载体
using EntityParams = std::variant<
    std::monostate,          // 无参数（保留位）
    PlayerParams,
    BackgroundParams,
    CameraParams,
    UIParams
>;
```

## 工厂类设计

### 职责

- 接收 `EntityType` + `EntityParams`，分配对应的实体对象
- 根据参数结构体设置实体属性（world_pos、texture_path 等）
- 调用 `Initialize()`，失败时自动清理并返回 `nullptr`
- 记录创建过程的 spdlog 日志

### API

```cpp
class EntityFactory {
public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    EntityFactory(const EntityFactory&) = delete;
    EntityFactory& operator=(const EntityFactory&) = delete;

    /**
     * 创建实体：分配内存 → 设置属性 → Initialize() → 返回实体指针
     *
     * @param type  实体类型
     * @param params 创建参数（std::variant，类型不匹配时返回 nullptr）
     * @return 创建成功返回实体指针（调用方负责 delete），失败返回 nullptr
     */
    [[nodiscard]] Object* Create(EntityType type, const EntityParams& params) const;
};
```

### Create() 实现骨架

```cpp
Object* EntityFactory::Create(const EntityType type, const EntityParams& params) const {
    Object* entity = nullptr;

    switch (type) {
        case EntityType::Player: {
            if (!std::holds_alternative<PlayerParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 PlayerParams");
                return nullptr;
            }
            const auto& p = std::get<PlayerParams>(params);

            auto* player = new Player();
            // player->SetName(p.name);   // TODO: Player 类实现 SetName 后启用
            player->SetWorldPos(p.world_pos);

            entity = player;
            break;
        }

        case EntityType::Background: {
            if (!std::holds_alternative<BackgroundParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 BackgroundParams");
                return nullptr;
            }
            const auto& p = std::get<BackgroundParams>(params);

            auto* bg = new Background();
            bg->SetPath(p.texture_path);
            bg->SetWorldPos(p.world_pos);

            entity = bg;
            break;
        }

        case EntityType::Camera: {
            if (!std::holds_alternative<CameraParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 CameraParams");
                return nullptr;
            }
            const auto& p = std::get<CameraParams>(params);

            auto* cam = new Camera();
            cam->SetWorldPos(p.world_pos);

            entity = cam;
            break;
        }

        case EntityType::UserInterface: {
            if (!std::holds_alternative<UIParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 UIParams");
                return nullptr;
            }
            const auto& p = std::get<UIParams>(params);

            auto* ui = new UserInterface();
            ui->SetPath(p.texture_path);
            ui->SetScreenPos(p.screen_pos);

            entity = ui;
            break;
        }

        default:
            spdlog::error("[EntityFactory] 未知实体类型: {}", static_cast<int>(type));
            return nullptr;
    }

    // 统一初始化
    if (entity->Initialize() != 0) {
        spdlog::error("[EntityFactory] 实体 Initialize() 失败，已清理");
        delete entity;
        return nullptr;
    }

    spdlog::info("[EntityFactory] 创建实体成功: type={}", static_cast<int>(type));
    return entity;
}
```

### 便捷方法（可选）

对于高频使用的实体类型，可以添加语义化的便捷包装：

```cpp
/// 快捷创建 Background
[[nodiscard]] Background* CreateBackground(const std::string& path, glm::vec2 pos) {
    return static_cast<Background*>(Create(
        EntityType::Background,
        BackgroundParams{path, pos}
    ));
}

/// 快捷创建 Camera
[[nodiscard]] Camera* CreateCamera(glm::vec2 pos) {
    return static_cast<Camera*>(Create(
        EntityType::Camera,
        CameraParams{pos}
    ));
}
```

## SceneMain 改造对比

### 改造前（当前代码：`src/SceneMain.cpp:8-38`）

```cpp
int SceneMain::Initialize() {
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;

    m_camera = new Camera();
    if (m_camera->Initialize()) {
        m_return_code = -1;
        goto to_quit;
    }
    m_camera->SetWorldPos((m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2));

    m_current_background = new Background();
    m_current_background->SetPath("assets/images/backgrounds/purple.png");
    if (m_current_background->Initialize()) {
        m_return_code = -1;
        goto to_quit;
    }
    m_current_background->SetWorldPos(glm::vec2{0,0});
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}
```

### 改造后

```cpp
int SceneMain::Initialize() {
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;

    auto cam_pos = (m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2);
    m_camera = m_factory.CreateCamera(cam_pos);
    if (!m_camera) {
        m_return_code = -1;
        goto to_quit;
    }

    m_current_background = m_factory.CreateBackground(
        "assets/images/backgrounds/purple.png",
        glm::vec2{0, 0}
    );
    if (!m_current_background) {
        m_return_code = -1;
        goto to_quit;
    }
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}
```

主要变化：
- `new` + `Initialize()` 两步合一，嵌套层级减少
- 错误检查统一为 `if (!ptr)` 判空
- setter 调用内聚到工厂内部

## 类的存放与注册

### 文件位置

```
src/core/
  ├─ EntityFactory.h      ← 枚举 + 参数结构体 + 工厂声明
  └─ EntityFactory.cpp    ← Create() 实现
```

### CMakeLists.txt

在 `add_executable(escape-from-lily ...)` 的源文件列表中加入 `src/core/EntityFactory.cpp`。

### SceneMain 集成

**SceneMain.h 新增成员：**

```cpp
#include "core/EntityFactory.h"
// ...
class SceneMain : public Scene {
private:
    EntityFactory m_factory;    // 简单工厂实例
    // ...
};
```

## 实现步骤

1. 创建 `src/core/EntityFactory.h` — 枚举、参数结构体、`EntityParams` variant、`EntityFactory` 类声明
2. 创建 `src/core/EntityFactory.cpp` — `Create()` 分发实现
3. 将 `EntityFactory.cpp` 加入 `CMakeLists.txt`
4. 修改 `src/SceneMain.h` — 添加 `#include "core/EntityFactory.h"` 和 `EntityFactory m_factory` 成员
5. 修改 `src/SceneMain.cpp` — `Initialize()` 改为工厂调用，`Quit()` 无需改动（`delete` 照旧）
6. 构建验证

## 扩展路线（未来）

- **Registry 注册表** — 不再 hard-code `switch-case`，每种实体在工厂中注册一个 lambda/函数指针，新增实体只需一行注册调用，无需修改工厂源码
- **原型模式** — 工厂持有每种实体的"原型对象"，`Create()` 通过 clone 产生新实例，适合需要大量同构实体（如子弹、粒子）的场景
- **对象池** — 工厂不 delete 而是回收实体到池中，`Create()` 优先复用闲置对象，适合频繁创建/销毁的实体
- **配置驱动** — 从 JSON/TOML 读取实体类型和参数，实现数据驱动的关卡加载

## 参考

- 项目现有实体层次：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity` → `MovableEntity` → `Player`（见 AGENTS.md）
- 生命周期惯例：构造 → `Initialize()` → `Quit()` → 析构（见 `Object.h`）
- 错误处理模式：`goto to_quit` + `EFL_ClassInit` / `EFL_ClassQuit`（见 `Def.h` 和 `SceneMain.cpp`）
- `SceneMain.cpp:17` 处已有 `// TODO: 使用工厂方法重构` 标记
