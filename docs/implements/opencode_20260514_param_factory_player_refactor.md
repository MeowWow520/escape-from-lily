# Player 参数系统重构：Param\<T\> + 工厂全面接管初始化

## 目标

1. 定义 `Param<T>` 包装器，支持 `DEFAULT` / `AS_PATH` / 自定义值三种语义
2. 扩容 `PlayerParams`，覆盖继承链所有 protected 成员
3. 工厂接管纹理加载和所有参数设置，`Initialize()` 仅做轻量链式调用
4. 调用方可用 C++20 设计初始化器内联传参，只写非默认值

---

## 改动清单

| 操作 | 文件 |
|---|---|
| **新建** | `src/core/Factory/Param.h` |
| **修改** | `src/core/Factory/EntityTypes.h` |
| **修改** | `src/core/Factory/EntityFactory.h` |
| **修改** | `src/core/Factory/EntityFactory.cpp` |
| **修改** | `src/core/Entities/Player.cpp` |
| **修改** | `src/core/Entities/Base/MovableEntity.cpp` |
| **修改** | `src/core/Entities/Base/HealthyPointEntity.h` |
| **修改** | `src/core/Entities/Base/HealthyPointEntity.cpp` |
| **修改** | `src/SceneMain.cpp` |

---

## Step 1：新建 `src/core/Factory/Param.h`

```cpp
//
// Created by MeowWow520 on 2026/7/8.
//

#ifndef ESCAPE_FROM_LILY_PARAM_H
#define ESCAPE_FROM_LILY_PARAM_H

#include <cstdint>

struct DefaultTag {};
inline constexpr DefaultTag DEFAULT{};

struct TextureTag {};
inline constexpr TextureTag AS_PATH{};

enum class ParamSource : uint8_t { Default, FromTexture, Custom };

template<typename T>
struct Param {
    ParamSource source = ParamSource::Default;
    T value{};

    Param() = default;
    Param(DefaultTag) : source(ParamSource::Default) {}
    Param(TextureTag) : source(ParamSource::FromTexture) {}
    Param(const T& v) : source(ParamSource::Custom), value(v) {}
    Param(T&& v) noexcept : source(ParamSource::Custom), value(std::move(v)) {}
};

#endif
```

---

## Step 2：修改 `src/core/Factory/EntityTypes.h`

- 顶栏新增 `#include "Param.h"`
- `PlayerParams` 替换为如下定义：

```cpp
struct PlayerParams {
    std::string m_player_name;
    std::string texture_path;

    Param<glm::vec2> world_pos{DEFAULT};
    Param<glm::vec2> pivot{DEFAULT};
    Param<glm::vec2> texture_size{AS_PATH};
    Param<glm::vec2> scale{DEFAULT};
    Param<glm::vec2> hitbox{DEFAULT};
    Param<float> max_speed{DEFAULT};
    Param<float> acceleration{DEFAULT};
    Param<float> health{DEFAULT};
    Param<float> get_damage{DEFAULT};
    Param<bool> visible{DEFAULT};
    Param<float> rotation{DEFAULT};
    Param<SDL_Color> color{DEFAULT};
    Param<SDL_BlendMode> blend_mode{DEFAULT};
    Param<SDL_Rect> rect{DEFAULT};
};
```

设计初始化器调用示例：

```cpp
EntityFactory::createPlayer(PlayerParams{
    .m_player_name = "Lily",
    .texture_path = "assets/lily.png",
    .health = 200.0f
});
```

未提及的字段自动使用 `=` 右侧的默认值（`DEFAULT` 或 `AS_PATH`）。

---

## Step 3：修改 `src/core/Factory/EntityFactory.h`

```cpp
// 改前
static EntityPtr<Player> createPlayer(const EntityParams& params);

// 改后
static EntityPtr<Player> createPlayer(const PlayerParams& params);
```

其他工厂方法（`createCamera`、`createBackground` 等）保持 `EntityParams` 签名不变。

---

## Step 4：修改 `src/core/Factory/EntityFactory.cpp`

新增包含：

```cpp
#include <SDL3_image/SDL_image.h>
```

（`IMG_LoadTexture` 移至工厂处理，Player::Initialize 中不再需要）

`createPlayer` 完整实现：

```cpp
EntityPtr<Player> EntityFactory::createPlayer(const PlayerParams& params) {
    EntityPtr<Player> entity(new Player());

    // — 基础参数 —
    entity->setName(params.m_player_name);
    entity->setPath(params.texture_path);

    // — Custom 值直接 set —
    if (params.max_speed.source == ParamSource::Custom)
        entity->setMaxSpeed(params.max_speed.value);
    if (params.acceleration.source == ParamSource::Custom)
        entity->setAcceleration(params.acceleration.value);
    if (params.health.source == ParamSource::Custom)
        entity->setHealth(params.health.value);
    if (params.get_damage.source == ParamSource::Custom)
        entity->setGetDamage(params.get_damage.value);
    if (params.pivot.source == ParamSource::Custom)
        entity->setPivot(params.pivot.value);
    if (params.scale.source == ParamSource::Custom)
        entity->setScale(params.scale.value);
    if (params.hitbox.source == ParamSource::Custom)
        entity->setHitbox(params.hitbox.value);
    if (params.rotation.source == ParamSource::Custom)
        entity->setRotation(params.rotation.value);
    if (params.color.source == ParamSource::Custom)
        entity->setColor(params.color.value);
    if (params.blend_mode.source == ParamSource::Custom)
        entity->setBlendMode(params.blend_mode.value);
    if (params.rect.source == ParamSource::Custom)
        entity->setRect(params.rect.value);

    // — DEFAULT 配置兜底 —
    if (params.max_speed.source == ParamSource::Default)
        entity->setMaxSpeed(ConfigManager::getInstance().getPlayerJson().max_speed);
    if (params.acceleration.source == ParamSource::Default
        && ConfigManager::getInstance().getDefaultJson().feature.acceleration)
        entity->setAcceleration(0.0f);
    if (params.visible.source == ParamSource::Default)
        entity->setVisible(true);

    // — 加载纹理（工厂接管） —
    SDL_Texture* tex = IMG_LoadTexture(
        Game::getInstance().getSDLRenderer(),
        params.texture_path.c_str());
    if (!tex) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "IMG_LoadTexture failed: {}", SDL_GetError());
        return nullptr;
    }
    entity->setTexture(tex);

    // — texture_size 后处理 —
    if (params.texture_size.source == ParamSource::FromTexture) {
        glm::vec2 size;
        SDL_GetTextureSize(tex, &size.x, &size.y);
        entity->setTextureSize(size);
    } else if (params.texture_size.source == ParamSource::Custom) {
        entity->setTextureSize(params.texture_size.value);
    }

    // — world_pos 后处理 —
    if (params.world_pos.source == ParamSource::Default) {
        glm::vec2 worldSize = Game::getInstance().getCurrentScene()->GetWorldSize();
        entity->setWorldPos((worldSize - entity->getTextureSize()) / 2.0f);
    } else if (params.world_pos.source == ParamSource::Custom) {
        entity->setWorldPos(params.world_pos.value);
    }

    // — Initialize —
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize Player failed");
        return nullptr;
    }

    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::Player");
    return entity;
}
```

---

## Step 5：精简 `src/core/Entities/Player.cpp`

```cpp
int Player::Initialize() {
    m_vec2_screenPos = transScreenPos();
    return HealthyPointEntity::Initialize();
}
```

删掉 `#include <SDL3_image/SDL_image.h>`。如果 `#include <algorithm>` 不再使用（clamp 已移除或移至别处）也一并删除。

原来的 `Initialize` 做了以下工作，全部移至工厂：
- `m_fMaxSpeed = config...` → 工厂 Step 4 中 DEFAULT 处理
- `m_bVisible = true` → 工厂 Step 4 中 DEFAULT 处理
- `IMG_LoadTexture` → 工厂 Step 4 中加载
- `SDL_GetTextureSize` → 工厂 Step 4 中 AS_PATH 处理
- `m_vec2_worldPos = (worldSize - texSize) / 2` → 工厂 Step 4 中 world_pos 后处理

---

## Step 6：精简 `src/core/Entities/Base/MovableEntity.cpp`

```cpp
int MovableEntity::Initialize() {
    return 0;
}
```

删除 `m_fMaxSpeed = m_config_manager.getPlayerJson().max_speed;`（工厂 Step 4 中读取 config 兜底）。

删除 acceleration feature flag 逻辑。

---

## Step 7：给 `HealthyPointEntity` 添加 setter/getter

### `HealthyPointEntity.h`

`public:` 段新增：

```cpp
float setHealth(float health);
[[nodiscard]] float getHealth() const;
float setGetDamage(float dmg);
[[nodiscard]] float getGetDamage() const;
```

### `HealthyPointEntity.cpp`

```cpp
float HealthyPointEntity::setHealth(const float health) {
    m_fHealth = health;
    return health;
}

[[nodiscard]] float HealthyPointEntity::getHealth() const {
    return m_fHealth;
}

float HealthyPointEntity::setGetDamage(const float dmg) {
    m_fGetDamage = dmg;
    return dmg;
}

[[nodiscard]] float HealthyPointEntity::getGetDamage() const {
    return m_fGetDamage;
}
```

---

## Step 8：更新 `src/SceneMain.cpp`

当前写法（创建临时变量）：

```cpp
PlayerParams player = {
    m_config_manager.getPlayerJson().default_name,
    m_config_manager.getPlayerJson().texture_path
};
m_player = EntityFactory::createPlayer(player);
```

改为内联设计初始化器（无需临时变量，未提及字段走默认值）：

```cpp
m_player = EntityFactory::createPlayer({
    .m_player_name = m_config_manager.getPlayerJson().default_name,
    .texture_path = m_config_manager.getPlayerJson().texture_path,
    .health = 200.0f
});
```

`texture_path` 是必填字段（方案 A），其余字段如 `max_speed`、`pivot`、`texture_size` 等自动走 `PlayerParams` 中的默认值。

如果未来想只写非默认参数：

```cpp
m_player = EntityFactory::createPlayer({
    .m_player_name = "Lily",
    .texture_path = "assets/lily.png",
    .health = 200.0f,
    .max_speed = 1000.0f
});
```

---

## 关键技术原理

### 为什么 `PlayerParams{.a = 1, .b = 2}` 可以直接传参？

1. **默认成员初始化器**：`Param<glm::vec2> pivot{DEFAULT}` — 未在初始化器中提及的字段使用 `=` 后的默认值
2. **隐式转换**：`PlayerParams` 右值通过 `const PlayerParams&` 参数绑定，编译器延长临时对象生命周期至函数调用结束
3. **无需 `EntityParams` 包装**：改用 `const PlayerParams&` 后跳过了 `std::variant` 的运行时检查，编译期即可确定类型

### 为什么纹理加载要搬到工厂？

方案 B 的核心考量：
- **单一责任**：工厂负责"如何用参数组装实体"，实体负责"运行时行为"
- **后处理依赖**：`AS_PATH` 需要纹理加载后的 `SDL_GetTextureSize`，`world_pos` 的 `DEFAULT`（自动居中）需要知道纹理尺寸，两者都必须在 `Initialize()` 之前或之后由工厂协调顺序
- **避免 Initialize 覆盖**：原来 `Initialize()` 硬编码覆盖 `m_fMaxSpeed` 和 `m_vec2_worldPos`，工厂先设 Custom 值后调 `Initialize()` 可保证不被覆盖

### 为什么需要 `Param<T>` 而非 `std::optional`？

| 场景 | `std::optional<T>` | `Param<T>` |
|---|---|---|
| "用类的默认值" | `std::nullopt` | `DEFAULT`（`source=Default`） |
| "从纹理推导" | ❌ 无法表达 | `AS_PATH`（`source=FromTexture`） |
| "自定义值" | `T` | `T`（`source=Custom`） |

`Param<T>` 比 `optional` 多了一种 `FromTexture` 语义，且源代码意图更明确。

---

## 继承链成员对应表

| 类 | 成员 | `PlayerParams` 字段 | 默认语义 |
|---|---|---|---|
| Object | `m_game_instance` | —（不可配） | — |
| Object | `m_config_manager` | —（不可配） | — |
| Object | `m_cEntityName` | —（硬编码"Player"） | — |
| ObjectWorld | `m_vec2_worldPos` | `world_pos` | `DEFAULT` → 自动居中 |
| ObjectScreen | `m_vec2_screenPos` | —（由 transScreenPos 计算） | — |
| TexturedEntity | `m_texture` | —（工厂加载） | — |
| TexturedEntity | `m_colorMod` | `color` | `DEFAULT` → 0xFFFFFFFF |
| TexturedEntity | `m_blendMode` | `blend_mode` | `DEFAULT` → SDL_BLENDMODE_BLEND |
| TexturedEntity | `m_bTextureDirty` | —（内部使用） | — |
| TexturedEntity | `m_bVisible` | `visible` | `DEFAULT` → true |
| TexturedEntity | `m_fRotation` | `rotation` | `DEFAULT` → 0.0 |
| TexturedEntity | `m_strPath` | `texture_path` | **必填**（无默认） |
| TexturedEntity | `m_rect` | `rect` | `DEFAULT` → {0,0,0,0} |
| TexturedEntity | `m_vec2_textureSize` | `texture_size` | `AS_PATH` → 从纹理查询 |
| TexturedEntity | `m_vec2_scale` | `scale` | `DEFAULT` → {1,1} |
| TexturedEntity | `m_vec2_pivot` | `pivot` | `DEFAULT` → {0.5,0.5} |
| TexturedEntity | `m_vec2_entityHitbox` | `hitbox` | `DEFAULT` → {1,1} |
| MovableEntity | `m_fMaxSpeed` | `max_speed` | `DEFAULT` → 读 player.json |
| MovableEntity | `m_fAcceleration` | `acceleration` | `DEFAULT` → 0（或 feature 控制） |
| MovableEntity | `m_vec2_vector` | —（运动方向，运行时计算） | — |
| HealthyPointEntity | `m_fHealth` | `health` | `DEFAULT` → 0.0 |
| HealthyPointEntity | `m_fGetDamage` | `get_damage` | `DEFAULT` → 0.0 |
| Player | `m_strPlayerName` | `m_player_name` | **必填**（无默认） |
