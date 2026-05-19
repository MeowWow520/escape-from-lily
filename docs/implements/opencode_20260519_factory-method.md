# 简单工厂（Simple Factory）设计方案

> 设计日期：2026/5/19
> 来源：OpenCode 会话规划

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| 工厂模式 | **简单工厂（参数化创建）** | 实体类型有限（~4 种），无需 GoF Factory Method 的 Creator 继承体系 |
| 创建流程 | **两阶段：构造 → Initialize** | 遵循项目现有生命周期惯例（见 AGENTS.md / Object.h） |
| 错误处理 | **返回 nullptr（空 unique_ptr）** | 与 Initialize() 返回 0/非0 一致；调用方检查 `.get()` 或 `operator bool` |
| 参数传递 | **按类型定义参数结构体** | 每个实体参数不同（Background 需要 path，Player 需要 name），结构体比零散参数更清晰 |
| 所有权 | **返回 `std::unique_ptr<Object>`** | 编译期强制所有权，消除 `delete` 遗漏；项目已有 `TexturePtr` 先例（`unique_ptr + SDL_DestroyTexture` deleter） |

## 模式概述

简单工厂将"对象创建 + 初始化"封装到一个类中，对外暴露统一的 `Create()` 接口，内部根据传入的实体类型分发到对应的构造逻辑。

```
调用方（Scene）
  │
  ▼
EntityFactory::Create(EntityType, Params)
  │
  ├─ case Player:    → make_unique<Player>()   → SetName() → Initialize()
  ├─ case Background → make_unique<Background>() → SetPath() → Initialize()
  ├─ case Camera:    → make_unique<Camera>()   → Initialize()
  └─ case UI:        → make_unique<UserInterface>() → Initialize()
```

对比当前 `SceneMain::Initialize()` 中的做法（`new` → 手动调 setter → `Initialize` → 检查返回值 → `goto to_quit`），工厂方法将这一段重复模板内聚，减少出错和代码冗余。

工厂返回 `unique_ptr` 进一步消除裸指针：构造失败时 `unique_ptr` 自动析构，无需手动 `delete`；销毁时 `unique_ptr.reset()` 一步到位，不会漏删。

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

每种实体有自己的参数结构体。工厂通过 `std::variant` 多态传参，类型安全。

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

## 指针类型别名

```cpp
/// 实体指针：unique_ptr<Object>（无自定义 deleter，销毁前需手动调 Quit()）
template <typename T = Object>
using EntityPtr = std::unique_ptr<T>;
```

> 注意：不做自定义 deleter（自动 `Quit() + delete`），因为项目需要检查 `Quit()` 的返回值（`goto to_quit` + `EFL_ClassQuit`）。销毁时调用方先显式 `Quit()` 检查返回值，再 `reset()`。

## 工厂类设计

### 职责

- 接收 `EntityType` + `EntityParams`，分配对应的实体对象
- 根据参数结构体设置实体属性（world_pos、texture_path 等）
- 调用 `Initialize()`，失败时返回空 `unique_ptr`（对象自动析构）
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
     * 创建实体：分配内存 → 设置属性 → Initialize() → 返回 EntityPtr
     *
     * @param type  实体类型
     * @param params 创建参数（std::variant，类型不匹配时返回空 EntityPtr）
     * @return 创建成功返回 unique_ptr<Object>，失败返回 nullptr
     */
    [[nodiscard]] EntityPtr<Object> Create(EntityType type, const EntityParams& params) const;
};
```

### Create() 实现骨架

```cpp
EntityPtr<Object> EntityFactory::Create(const EntityType type, const EntityParams& params) const {
    EntityPtr<Object> entity;

    switch (type) {
        case EntityType::Player: {
            if (!std::holds_alternative<PlayerParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 PlayerParams");
                return nullptr;
            }
            const auto& p = std::get<PlayerParams>(params);

            auto player = std::make_unique<Player>();
            // player->SetName(p.name);   // TODO: Player 类实现 SetName 后启用
            player->SetWorldPos(p.world_pos);

            entity = std::move(player);
            break;
        }

        case EntityType::Background: {
            if (!std::holds_alternative<BackgroundParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 BackgroundParams");
                return nullptr;
            }
            const auto& p = std::get<BackgroundParams>(params);

            auto bg = std::make_unique<Background>();
            bg->SetPath(p.texture_path);
            bg->SetWorldPos(p.world_pos);

            entity = std::move(bg);
            break;
        }

        case EntityType::Camera: {
            if (!std::holds_alternative<CameraParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 CameraParams");
                return nullptr;
            }
            const auto& p = std::get<CameraParams>(params);

            auto cam = std::make_unique<Camera>();
            cam->SetWorldPos(p.world_pos);

            entity = std::move(cam);
            break;
        }

        case EntityType::UserInterface: {
            if (!std::holds_alternative<UIParams>(params)) {
                spdlog::error("[EntityFactory] 参数类型不匹配: 期望 UIParams");
                return nullptr;
            }
            const auto& p = std::get<UIParams>(params);

            auto ui = std::make_unique<UserInterface>();
            ui->SetPath(p.texture_path);
            ui->SetScreenPos(p.screen_pos);

            entity = std::move(ui);
            break;
        }

        default:
            spdlog::error("[EntityFactory] 未知实体类型: {}", static_cast<int>(type));
            return nullptr;
    }

    // 统一初始化 —— 失败时 unique_ptr 自动析构，无需手动 delete
    if (entity->Initialize() != 0) {
        spdlog::error("[EntityFactory] 实体 Initialize() 失败，已自动清理");
        return nullptr;
    }

    spdlog::info("[EntityFactory] 创建实体成功: type={}", static_cast<int>(type));
    return entity;
}
```

> 注意：`Initialize()` 失败时直接 `return nullptr`，`entity` 作为局部 `unique_ptr` 离开作用域自动调用析构函数（已构造的对象成员会正常销毁）。**不需要**像裸指针版那样写 `delete entity`。

### 便捷方法（可选）

便捷方法返回具体类型的 `unique_ptr`，调用方无需 `static_cast`：

```cpp
/// 快捷创建 Background
[[nodiscard]] EntityPtr<Background> CreateBackground(const std::string& path, glm::vec2 pos) {
    auto obj = Create(EntityType::Background, BackgroundParams{path, pos});
    return EntityPtr<Background>(static_cast<Background*>(obj.release()));
}

/// 快捷创建 Camera
[[nodiscard]] EntityPtr<Camera> CreateCamera(glm::vec2 pos) {
    auto obj = Create(EntityType::Camera, CameraParams{pos});
    return EntityPtr<Camera>(static_cast<Camera*>(obj.release()));
}

/// 快捷创建 Player
[[nodiscard]] EntityPtr<Player> CreatePlayer(const std::string& name, glm::vec2 pos) {
    auto obj = Create(EntityType::Player, PlayerParams{name, pos});
    return EntityPtr<Player>(static_cast<Player*>(obj.release()));
}
```

`obj.release()` 交出所有权后转为具体类型的 `unique_ptr`，零开销安全转换。

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

**SceneMain.h 成员变更：**

```cpp
// 裸指针 → unique_ptr
Camera* m_camera{};                          // 改造前
EntityPtr<Camera> m_camera;                  // 改造后

Background* m_current_background{};          // 改造前
EntityPtr<Background> m_current_background;  // 改造后
```

**Initialize()：**

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

**Quit()：**

```cpp
int SceneMain::Quit() {
    // Background —— 先 Quit() 检查返回值，再 reset() 释放
    if (m_current_background) {
        if (m_current_background->Quit()) {
            m_return_code = -1;
            goto to_quit_early;
        }
        m_current_background.reset();
    }

    // Camera
    if (m_camera) {
        if (m_camera->Quit()) {
            m_return_code = -1;
            goto to_quit_early;
        }
        m_camera.reset();
    }

to_quit_early:
    const ssl loc = ssl::current();
    return EFL_ClassQuit(Scene::Quit(), loc);
}
```

> Note: `goto to_quit_early` 保证 `Quit()` 失败时能提前返回错误码。此时尚未 `reset()` 的 `unique_ptr` 在 `Quit()` 返回前仍有效，离开作用域后自动析构（但对应的 SDL 资源可能处于半清理状态——建议 `Quit()` 失败后不再继续使用该实体）。

主要变化：
- `new` + `Initialize()` 两步合一，嵌套层级减少
- 错误检查统一为 `if (!ptr)` 判空（`unique_ptr` 支持 `operator bool`）
- setter 调用内聚到工厂内部
- `Quit()` 中 `reset()` 自动 `delete`，不再需要写 `delete m_camera`
- `unique_ptr` 持有期间访问成员方式不变（`m_camera->Update(dt)` 照常）

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

**SceneMain.h 修改：**

```cpp
#include "core/EntityFactory.h"
// ...
class SceneMain : public Scene {
public:
    // ...
private:
    EntityFactory m_factory;
    EntityPtr<Background> m_current_background;  // 原 Background*
    EntityPtr<Camera> m_camera;                  // 原 Camera*
};
```

**GetCamera() 适配：**

由于 `m_camera` 现在是 `unique_ptr<Camera>`，`GetCamera()` 仍可返回裸指针（Scene 拥有 Camera 所有权，返回裸引用指针不转移所有权）：

```cpp
Camera* SceneMain::GetCamera() {
    return m_camera.get();   // .get() 返回裸指针，不转移所有权
}
```

## 实现步骤

1. 创建 `src/core/EntityFactory.h` — 枚举、参数结构体、`EntityPtr` 别名、`EntityParams` variant、`EntityFactory` 类声明
2. 创建 `src/core/EntityFactory.cpp` — `Create()` 分发实现（使用 `std::make_unique`）
3. 将 `EntityFactory.cpp` 加入 `CMakeLists.txt`
4. 修改 `src/SceneMain.h` — 添加 `#include`，`m_current_background` / `m_camera` 改为 `EntityPtr<T>`，新增 `EntityFactory m_factory` 成员
5. 修改 `src/SceneMain.cpp` — `Initialize()` 改为工厂调用；`Quit()` 改为 `Quit()` 检查 + `reset()`；`GetCamera()` 改为 `.get()`
6. 构建验证

## 扩展路线（未来）

- **Registry 注册表** — 不再 hard-code `switch-case`，每种实体在工厂中注册一个 lambda/函数指针，新增实体只需一行注册调用，无需修改工厂源码
- **原型模式** — 工厂持有每种实体的"原型对象"，`Create()` 通过 clone 产生新实例，适合需要大量同构实体（如子弹、粒子）的场景
- **对象池** — 工厂不销毁而是回收实体到池中，`Create()` 优先复用闲置对象，适合频繁创建/销毁的实体
- **配置驱动** — 从 JSON/TOML 读取实体类型和参数，实现数据驱动的关卡加载

## 参考

- 项目现有实体层次：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity` → `MovableEntity` → `Player`（见 AGENTS.md）
- 生命周期惯例：构造 → `Initialize()` → `Quit()` → 析构（见 `Object.h`）
- 错误处理模式：`goto to_quit` + `EFL_ClassInit` / `EFL_ClassQuit`（见 `Def.h` 和 `SceneMain.cpp`）
- `SceneMain.cpp:17` 处已有 `// TODO: 使用工厂方法重构` 标记
- 项目已有智能指针先例：`TexturedEntity::TexturePtr` = `std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>`
