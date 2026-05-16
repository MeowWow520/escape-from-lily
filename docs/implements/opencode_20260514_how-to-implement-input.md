# Input 基类设计方案

> 设计日期：2026/5/14
> 来源：OpenCode 会话规划

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| 继承关系 | **独立类**（不继承 Object） | 无需 SDL 资源初始化/清理，轻量级 |
| 默认绑定 | **构造函数中硬编码** | WASD + Escape，后续可加运行时重绑定 |
| 归属层级 | **Game 持有全局** | 所有 Scene 共享同一 Input，Game 负责事件分发 |
| 实现范围 | **键盘 + 完整动作系统** | Action 枚举 + 状态机 + KeyboardInput 一次性落地 |

## 枚举定义

```cpp
/// 游戏动作：设备无关的抽象指令
enum class Action : uint8_t {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Pause, Confirm, Cancel
};

/// 动作状态：每帧稳定态
enum class ActionState : uint8_t {
    Idle,      // 本帧无操作
    Pressed,   // 本帧刚按下（上升沿）
    Held,      // 持续按住中
    Released   // 本帧刚松开（下降沿）
};
```

## 类层次结构

```
Input (独立基类)
  └─ KeyboardInput  ← 键盘事件 → Action 映射
```

## Input 基类

### 职责
- 维护 `key → Action` 绑定表
- 维护每帧 `Action → ActionState` 状态快照
- 提供动作状态查询接口
- 驱动状态转移（Pressed→Held, Released→Idle）

### API 设计

```cpp
class Input {
protected:
    /// 按键绑定映射表
    std::unordered_map<SDL_Keycode, Action> m_key_bindings;
    /// 当前帧动作状态
    /// Pressed / Released 本身就是边缘信号，无需额外变量对比两帧
    std::unordered_map<Action, ActionState> m_action_states;

    /// 设置默认键位绑定（构造函数中调用）
    virtual void SetDefaultBindings();

public:
    Input() noexcept;
    virtual ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    /// 处理 SDL 原始事件，由 Game::HandleEvents 调用
    /// 子类覆盖此方法处理设备特定事件并调用 SetActionState
    virtual void HandleEvents(SDL_Event event) = 0;

    /// 每帧更新状态转移，由 Game::Update 调用
    virtual void Update(float dt);

    // ── 查询接口 ──

    /// 查询指定动作的原始状态
    [[nodiscard]] ActionState GetActionState(Action action) const;

    /// 本帧刚按下？
    [[nodiscard]] bool IsActionPressed(Action action) const;
    /// 持续按住中？
    [[nodiscard]] bool IsActionHeld(Action action) const;
    /// 本帧刚松开？
    [[nodiscard]] bool IsActionReleased(Action action) const;

    /// 由 MoveUp/Left/Down/Right 推算移动方向向量，范围为 [-1, 1]
    [[nodiscard]] glm::vec2 GetMovementAxis() const;

    // ── 绑定管理 ──

    /// 将 SDL 键码绑定到游戏动作
    void BindAction(SDL_Keycode key, Action action);
    /// 解绑动作
    void UnbindAction(Action action);

    // ── 受保护工具方法（子类调用） ──

protected:
    /// 子类事件处理中调用此方法设置动作状态
    void SetActionState(Action action, ActionState state);
};
```

### 状态转移逻辑（`Update`）

```
按下键:    Idle → Pressed  ──(下一帧 Update)──→ Held
松开键:    Held → Released ──(下一帧 Update)──→ Idle
```

`Update` 每帧扫描动作表：
- `m_action_states[A] == Pressed` → 转为 `m_action_states[A] = Held`
- `m_action_states[A] == Released` → 转为 `m_action_states[A] = Idle`

同时复制 `m_action_states` 到 `m_prev_states`。

### 默认绑定（基类 `SetDefaultBindings`）

```cpp
void Input::SetDefaultBindings() {
    BindAction(SDLK_W,     Action::MoveUp);
    BindAction(SDLK_A,     Action::MoveLeft);
    BindAction(SDLK_S,     Action::MoveDown);
    BindAction(SDLK_D,     Action::MoveRight);
    BindAction(SDLK_ESCAPE, Action::Pause);
}
```

## KeyboardInput 子类

### 职责
- 接收 `SDL_EVENT_KEY_DOWN` / `SDL_EVENT_KEY_UP`
- 查 `m_key_bindings` 映射到 Action
- 调用 `SetActionState` 更新状态

### 设计

```cpp
class KeyboardInput : public Input {
public:
    KeyboardInput() noexcept;
    ~KeyboardInput() override = default;

    void HandleEvents(SDL_Event event) override;
};
```

### HandleEvents 逻辑

```
收到 SDL_EVENT_KEY_DOWN:
  key = event.key.key
  if key 在 m_key_bindings 中有绑定 action:
    if m_action_states[action] == Idle    → 设为 Pressed
    // 如果已经 Held 则不变（防止系统重复 KEY_DOWN）

收到 SDL_EVENT_KEY_UP:
  key = event.key.key
  if key 在 m_key_bindings 中有绑定 action:
    if m_action_states[action] == Held 或 Pressed  → 设为 Released
```

## GetMovementAxis 逻辑

```cpp
glm::vec2 Input::GetMovementAxis() const {
    glm::vec2 axis{0.0f, 0.0f};
    if (IsActionHeld(Action::MoveLeft) || IsActionPressed(Action::MoveLeft))
        axis.x -= 1.0f;
    if (IsActionHeld(Action::MoveRight) || IsActionPressed(Action::MoveRight))
        axis.x += 1.0f;
    if (IsActionHeld(Action::MoveUp) || IsActionPressed(Action::MoveUp))
        axis.y -= 1.0f;  // 注意：SDL Y 轴向下为正，向上移动为负
    if (IsActionHeld(Action::MoveDown) || IsActionPressed(Action::MoveDown))
        axis.y += 1.0f;

    // 对角线归一化
    if (glm::length(axis) > 1.0f)
        axis = glm::normalize(axis);

    return axis;
}
```

## 集成到现有项目

### Game 类修改

**Game.h 新增：**
```cpp
#include "core/Input/KeyboardInput.h"
// ...
class Game {
    // ...
private:
    KeyboardInput* m_input;  // 或 unique_ptr
public:
    [[nodiscard]] Input* GetInput() const;
    // ...
};
```

**Game::Initialize 新增：**
```cpp
m_input = new KeyboardInput();
```

**Game::HandleEvents 修改：**
```cpp
void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_input->HandleEvents(event);  // ← 新增：Input 先处理
        switch (event.type) {
            case SDL_EVENT_QUIT:
                spdlog::info("Receive SDL_EVENT_QUIT events, main loop quitting");
                m_running = false;
                break;
            default:
                m_current_scene->HandleEvents(event);
                break;
        }
    }
}
```

**Game::Update 修改：**
```cpp
void Game::Update(float dt) {
    m_input->Update(dt);           // ← 新增
    m_current_scene->Update(dt);
}
```

**Game::Quit 新增：**
```cpp
delete m_input;
m_input = nullptr;
```

### SceneMain 使用示例

```cpp
// SceneMain::Update
void SceneMain::Update(const float dt) {
    m_current_background->Update(dt);

    auto* input = m_game_instance.GetInput();
    glm::vec2 axis = input->GetMovementAxis();

    if (glm::length(axis) > 0.0f) {
        // 移动玩家（像素/秒）
        const float speed = 200.0f;
        m_player_position += axis * speed * dt;

        // 如果玩家将来是 MovableEntity：
        // m_player_entity->SetWorldPos(m_player_position);
    }

    // 系统动作
    if (input->IsActionPressed(Action::Pause)) {
        spdlog::info("Pause triggered");
        // 切换暂停状态...
    }
}
```

## 实现步骤

1. 重写 `src/core/Input/Input.h` — 基类定义（Action、ActionState、API）
2. 重写 `src/core/Input/Input.cpp` — 基类实现（Update、GetMovementAxis、绑定管理）
3. 重写 `src/core/Input/KeyboardInput.h` — KeyboardInput 声明
4. 重写 `src/core/Input/KeyboardInput.cpp` — HandleEvents 实现
5. 修改 `src/core/Game.h` — 添加 `m_input` 成员 + `GetInput()` getter
6. 修改 `src/core/Game.cpp` — Initialize/HandleEvents/Update/Quit 集成
7. 修改 `src/SceneMain.cpp` — 接入 Input 驱动玩家移动
8. 构建验证

## 扩展路线（未来）

- **MouseInput** — 鼠标位置、滚轮、按键映射到 Action（瞄准、射击）
- **GamepadInput** — SDL3 Gamepad API 映射
- **绑定配置文件** — JSON/TOML 加载键位绑定，支持用户自定义
- **Action 组合** — 如 Ctrl+Shift+W 的组合触发

## 参考

- 项目现有 `MovableEntity::m_movement` 结构体处理移动方向，Input 的 `GetMovementAxis` 为其上游数据源
- 生命周期遵循项目惯例（无 Initialize/Quit 的独立类由 Game 直接管理）
- 日志使用 `spdlog`（见 `Def.h`）
