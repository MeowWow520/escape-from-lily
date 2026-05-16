# SDL3 事件体系参考

> 整理日期：2026/5/14
> 来源：OpenCode 会话规划，基于 SDL3 官方文档

## 事件循环回顾

```cpp
void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {   // 非阻塞轮询
        m_input->HandleEvents(event); // Input 优先消费输入事件
        switch (event.type) {
            case SDL_EVENT_QUIT: /* ... */ break;
            default: m_current_scene->HandleEvents(event); break;
        }
    }
}
```

每帧调用一次。`SDL_PollEvent` 一次一个出队，队列空时返回 false。

---

## SDL_Event 联合体访问速查

`SDL_Event` 是巨大联合体，按 `event.type` 判断后访问对应字段：

| 事件类别 | 读取字段 |
|----------|----------|
| 通用 | `event.common.type`、`event.common.timestamp` |
| 窗口 | `event.window.data1` / `data2`（语义因事件而异） |
| 键盘 | `event.key.key`（SDL_Keycode）、`event.key.scancode`、`event.key.down` |
| 鼠标按钮 | `event.button.button`、`event.button.down`、`event.button.x` / `y` |
| 鼠标移动 | `event.motion.x` / `y`、`event.motion.xrel` / `yrel` |
| 鼠标滚轮 | `event.wheel.x` / `y`（float）、`event.wheel.direction` |
| 游戏手柄按钮 | `event.gbutton.button`、`event.gbutton.down` |
| 游戏手柄轴 | `event.gaxis.axis`、`event.gaxis.value`（-32768~32767） |
| 拖放 | `event.drop.data`（UTF-8 路径/文本）、`event.drop.x` / `y` |
| 用户自定义 | `event.user.data1` / `data2`、`event.user.code` |

> **注意**：SDL3 中窗口事件的 `data1`/`data2` 含义随子类型变化（如 RESIZED 中为宽/高，MOVED 中为 x/y）。

---

## 事件全量列表

### 1. 应用程序事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_QUIT` | 用户请求退出（关闭按钮 / ⌘Q） | **已处理**：Game 设 `m_running = false` |
| `SDL_EVENT_TERMINATING` | OS 即将终止进程 | 移动端专用，桌面端可忽略 |
| `SDL_EVENT_LOW_MEMORY` | 系统内存不足 | 可在未来清理缓存 |
| `SDL_EVENT_WILL_ENTER_BACKGROUND` | 即将进入后台 | 移动端；桌面端可暂停渲染 |
| `SDL_EVENT_DID_ENTER_BACKGROUND` | 已进入后台 | 移动端 |
| `SDL_EVENT_WILL_ENTER_FOREGROUND` | 即将返回前台 | 移动端；桌面端恢复渲染 |
| `SDL_EVENT_DID_ENTER_FOREGROUND` | 已返回前台且可交互 | 移动端 |
| `SDL_EVENT_LOCALE_CHANGED` | 系统语言/区域变更 | 多语言支持时可刷新文本 |
| `SDL_EVENT_SYSTEM_THEME_CHANGED` | 系统明暗主题切换 | 未来支持暗色模式时使用 |

### 2. 显示器事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_DISPLAY_ORIENTATION` | 显示器方向改变（横屏/竖屏） |
| `SDL_EVENT_DISPLAY_ADDED` | 新显示器接入 |
| `SDL_EVENT_DISPLAY_REMOVED` | 显示器断开 |
| `SDL_EVENT_DISPLAY_MOVED` | 显示器位置变化（多屏） |
| `SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED` | 桌面分辨率/刷新率变化 |
| `SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED` | 当前显示模式变化 |
| `SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED` | 内容缩放比变化（HiDPI） |
| `SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED` | 可用区域变化（任务栏等） |

> 桌面端一般不需主动处理，SDL 自动适配。

### 3. 窗口事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_WINDOW_SHOWN` | 窗口已显示 | 初始化完成标志 |
| `SDL_EVENT_WINDOW_HIDDEN` | 窗口已隐藏 | — |
| `SDL_EVENT_WINDOW_EXPOSED` | 窗口需要重绘（data1=1 为实时缩放中） | 可按需优化渲染 |
| `SDL_EVENT_WINDOW_MOVED` | 窗口移动（data1=x, data2=y） | — |
| `SDL_EVENT_WINDOW_RESIZED` | **窗口大小改变**（data1=w, data2=h） | **需要处理**：更新世界边界 |
| `SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED` | 像素尺寸改变（HiDPI 相关） | — |
| `SDL_EVENT_WINDOW_MINIMIZED` | 窗口最小化 | **建议处理**：暂停渲染循环 |
| `SDL_EVENT_WINDOW_MAXIMIZED` | 窗口最大化 | 恢复渲染 |
| `SDL_EVENT_WINDOW_RESTORED` | 窗口从最小化/最大化恢复 | 恢复渲染 |
| `SDL_EVENT_WINDOW_MOUSE_ENTER` | 鼠标进入窗口 | UI hover 相关 |
| `SDL_EVENT_WINDOW_MOUSE_LEAVE` | 鼠标离开窗口 | UI hover 相关 |
| `SDL_EVENT_WINDOW_FOCUS_GAINED` | **键盘焦点获得** | **建议处理**：恢复输入/取消暂停 |
| `SDL_EVENT_WINDOW_FOCUS_LOST` | **键盘焦点丢失** | **建议处理**：自动暂停 |
| `SDL_EVENT_WINDOW_CLOSE_REQUESTED` | 窗口管理器请求关闭 | 可处理为 QUIT 或确认对话框 |
| `SDL_EVENT_WINDOW_ENTER_FULLSCREEN` | 进入全屏 | — |
| `SDL_EVENT_WINDOW_LEAVE_FULLSCREEN` | 退出全屏 | — |
| `SDL_EVENT_WINDOW_DESTROYED` | 窗口已销毁 | 退出循环 |
| `SDL_EVENT_WINDOW_DISPLAY_CHANGED` | 窗口移到另一显示器 | — |

> `RESIZED` 是必须处理的：需重新计算相机位置和世界边界。  
> `FOCUS_LOST` / `FOCUS_GAINED` 建议处理：失焦时暂停游戏，避免输入丢失。

### 4. 键盘事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_KEY_DOWN` | 按键按下 | **已由 Input 接管** |
| `SDL_EVENT_KEY_UP` | 按键松开 | **已由 Input 接管** |
| `SDL_EVENT_TEXT_EDITING` | 输入法组合字符（拼写中） | 文本框输入时用 |
| `SDL_EVENT_TEXT_INPUT` | 输入法确认字符 | 文本框/控制台输入 |
| `SDL_EVENT_KEYMAP_CHANGED` | 键盘布局变更 | 可刷新绑定映射 |
| `SDL_EVENT_KEYBOARD_ADDED` | 新键盘插入 | — |
| `SDL_EVENT_KEYBOARD_REMOVED` | 键盘移除 | — |
| `SDL_EVENT_SCREEN_KEYBOARD_SHOWN` | 触摸屏虚拟键盘显示 | — |
| `SDL_EVENT_SCREEN_KEYBOARD_HIDDEN` | 触摸屏虚拟键盘隐藏 | — |

> 本项目**所有键盘输入已由 Input 类统一处理**，Scene / Entity 不再直接接触键盘事件。  
> `TEXT_INPUT` 未来可用于游戏内控制台。

### 5. 鼠标事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_MOUSE_MOTION` | 鼠标移动（`x`/`y` 绝对，`xrel`/`yrel` 相对） | 鼠标选中/视角旋转 |
| `SDL_EVENT_MOUSE_BUTTON_DOWN` | 鼠标按键按下 | UI 点击检测 |
| `SDL_EVENT_MOUSE_BUTTON_UP` | 鼠标按键松开 | UI 点击释放 |
| `SDL_EVENT_MOUSE_WHEEL` | 鼠标滚轮（`wheel.x`/`y`，float） | 缩放/翻页 |
| `SDL_EVENT_MOUSE_ADDED` | 新鼠标插入 | — |
| `SDL_EVENT_MOUSE_REMOVED` | 鼠标移除 | — |

> 将来实现 MouseInput 类，与 KeyboardInput 平级。

### 6. 游戏手柄事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_GAMEPAD_AXIS_MOTION` | 摇杆/扳机轴移动 | GamepadInput 接管 |
| `SDL_EVENT_GAMEPAD_BUTTON_DOWN` | 手柄按钮按下 | GamepadInput 接管 |
| `SDL_EVENT_GAMEPAD_BUTTON_UP` | 手柄按钮松开 | GamepadInput 接管 |
| `SDL_EVENT_GAMEPAD_ADDED` | 手柄连接 | 热插拔检测 |
| `SDL_EVENT_GAMEPAD_REMOVED` | 手柄断开 | 热插拔检测 |
| `SDL_EVENT_GAMEPAD_REMAPPED` | 手柄映射更新 | 重读绑定 |
| `SDL_EVENT_GAMEPAD_TOUCHPAD_*` | 手柄触摸板 | Steam Controller / PS5 |
| `SDL_EVENT_GAMEPAD_SENSOR_UPDATE` | 手柄陀螺仪 | — |

> 将来 GamepadInput 子类处理所有手柄事件。

### 7. 摇杆事件（传统 Joystick，已半废弃）

SDLG3 推荐使用 Gamepad API，Joystick 仅用于非标准设备：

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_JOYSTICK_AXIS_MOTION` | 摇杆轴移动 |
| `SDL_EVENT_JOYSTICK_BUTTON_DOWN/UP` | 摇杆按钮 |
| `SDL_EVENT_JOYSTICK_HAT_MOTION` | 方向键（Hat） |
| `SDL_EVENT_JOYSTICK_ADDED/REMOVED` | 设备连接/断开 |
| `SDL_EVENT_JOYSTICK_BATTERY_UPDATED` | 电池状态更新 |

> 本项目暂不处理，统一走 Gamepad API。

### 8. 触控事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_FINGER_DOWN` | 手指按下 |
| `SDL_EVENT_FINGER_UP` | 手指抬起 |
| `SDL_EVENT_FINGER_MOTION` | 手指滑动 |
| `SDL_EVENT_FINGER_CANCELED` | 触摸中断（如来电） |

> 移动端专用，桌面端忽略。

### 9. 捏合手势事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_PINCH_BEGIN` | 双指捏合开始 |
| `SDL_EVENT_PINCH_UPDATE` | 捏合进行中（缩放比例等） |
| `SDL_EVENT_PINCH_END` | 捏合结束 |

> 移动端缩放操作，桌面端由滚轮代替。

### 10. 剪贴板事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_CLIPBOARD_UPDATE` | 剪贴板内容已改变 |

> 可用于监听外部复制，实现游戏内粘贴。

### 11. 拖放事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_DROP_FILE` | 文件拖放到窗口（`drop.data` = UTF-8 路径） | 加载存档/模组 |
| `SDL_EVENT_DROP_TEXT` | 文本拖放到窗口 | 文本输入 |
| `SDL_EVENT_DROP_BEGIN` | 拖放开始（进入窗口） | 视觉提示 |
| `SDL_EVENT_DROP_COMPLETE` | 拖放完成（离开窗口） | 清理提示 |
| `SDL_EVENT_DROP_POSITION` | 拖放过程中位置更新 | 视觉提示 |

### 12. 音频设备事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_AUDIO_DEVICE_ADDED` | 新音频设备插入 |
| `SDL_EVENT_AUDIO_DEVICE_REMOVED` | 音频设备移除 |
| `SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED` | 音频格式变更 |

> SDL3_mixer 可能自动处理，一般不需干预。

### 13. 传感器事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_SENSOR_UPDATE` | 加速度计/陀螺仪数据更新 |

> 移动端专用。

### 14. 压感笔事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_PEN_PROXIMITY_IN/OUT` | 笔接近/离开感应区域 |
| `SDL_EVENT_PEN_DOWN/UP` | 笔接触/抬起 |
| `SDL_EVENT_PEN_BUTTON_DOWN/UP` | 笔按钮 |
| `SDL_EVENT_PEN_MOTION` | 笔移动 |
| `SDL_EVENT_PEN_AXIS` | 笔压力/角度变化 |

> 绘图应用专用，本项目忽略。

### 15. 摄像头事件

| 事件类型 | 含义 |
|----------|------|
| `SDL_EVENT_CAMERA_DEVICE_ADDED/REMOVED` | 摄像头热插拔 |
| `SDL_EVENT_CAMERA_DEVICE_APPROVED/DENIED` | 摄像头权限结果 |

> 无摄像头功能，忽略。

### 16. 渲染事件

| 事件类型 | 含义 | 本项目用途 |
|----------|------|-----------|
| `SDL_EVENT_RENDER_TARGETS_RESET` | 渲染目标已重置 | 重建渲染目标 |
| `SDL_EVENT_RENDER_DEVICE_RESET` | 渲染设备重置（纹理需重建） | **重要**：恢复所有纹理 |
| `SDL_EVENT_RENDER_DEVICE_LOST` | 渲染设备丢失且无法恢复 | 弹出错误并退出 |

> `RENDER_DEVICE_RESET` 发生在驱动崩溃恢复（TDR）、全屏切换等。需重新加载所有 `SDL_Texture`。  
> 当前项目的 `TexturedEntity` 有 `SetTextureFromPath()` 方法，可通过此事件触发批量重载。

### 17. 用户自定义事件

| 事件类型 | 范围 | 用途 |
|----------|------|------|
| `SDL_EVENT_USER` ~ `SDL_EVENT_LAST` | 0x8000 ~ 0xFFFF | 游戏内自定义事件 |

用法：
```cpp
Uint32 MY_EVENT = SDL_RegisterEvents(1);  // 注册 1 个事件类型
SDL_Event ev;
SDL_zero(ev);
ev.type = MY_EVENT;
ev.user.data1 = some_entity_pointer;
SDL_PushEvent(&ev);  // 推入队列，主线程 Poll 消费
```

> 用途：实体间通信（"玩家死亡" → Scene 播放过场）、定时回调、线程安全的消息传递。

---

## 本项目处理分布图

```
SDL_PollEvent(&event)
│
├─ m_input->HandleEvents(event)       // Input 消费
│   ├─ SDL_EVENT_KEY_DOWN/UP          → 写入 m_action_states
│   └─ (未来) SDL_EVENT_MOUSE_*       → MouseInput
│   └─ (未来) SDL_EVENT_GAMEPAD_*     → GamepadInput
│
├─ switch(event.type)
│   ├─ SDL_EVENT_QUIT                 → Game::m_running = false
│   └─ default                        → m_current_scene->HandleEvents(event)
│
└─ Scene::HandleEvents(event)
    ├─ SDL_EVENT_WINDOW_RESIZED       → 重新计算相机/世界边界
    ├─ SDL_EVENT_WINDOW_FOCUS_LOST    → 自动暂停
    ├─ SDL_EVENT_WINDOW_FOCUS_GAINED  → 恢复
    ├─ SDL_EVENT_RENDER_DEVICE_RESET  → 重建纹理
    ├─ SDL_EVENT_DROP_FILE            → 加载存档/模组
    └─ SDL_EVENT_USER                 → 游戏内事件路由
```

---

## 常见处理模式

### 窗口缩放更新世界边界

```cpp
void SceneMain::HandleEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            int w = event.window.data1;
            int h = event.window.data2;
            m_world_size = glm::vec2(w, h) * m_world_scale;
            m_camera_pos = (m_world_size - glm::vec2(w, h)) / glm::vec2(2);
            break;
        }
    }
}
```

### 焦点丢失自动暂停

```cpp
case SDL_EVENT_WINDOW_FOCUS_LOST: {
    m_paused = true;
    spdlog::info("Window lost focus - game paused");
    break;
}
case SDL_EVENT_WINDOW_FOCUS_GAINED: {
    m_paused = false;
    spdlog::info("Window gained focus - game resumed");
    break;
}
```

### 渲染设备重置恢复纹理

```cpp
case SDL_EVENT_RENDER_DEVICE_RESET: {
    spdlog::warn("Render device reset - reloading textures");
    m_current_background->SetTextureFromPath();
    // 遍历所有 TexturedEntity 重载...
    break;
}
```

### 拖放加载存档

```cpp
case SDL_EVENT_DROP_FILE: {
    std::string path(event.drop.data);
    if (path.ends_with(".sav")) {
        LoadGame(path);
    }
    SDL_free(event.drop.data);  // SDL3 要求手动释放
    break;
}
```

---

## 事件 API 工具函数速览

| 函数 | 用途 |
|------|------|
| `SDL_PollEvent(&event)` | 非阻塞取事件，队列空返回 false |
| `SDL_WaitEvent(&event)` | 阻塞直到有事件（省电，不适合游戏主循环） |
| `SDL_PushEvent(&event)` | 向队列注入事件 |
| `SDL_RegisterEvents(count)` | 注册自定义事件，返回起始 ID |
| `SDL_FlushEvent(type)` | 清空指定类型的所有事件 |
| `SDL_FlushEvents(minType, maxType)` | 清空范围内事件 |
| `SDL_SetEventEnabled(type, enabled)` | 动态启用/禁用某类事件（减少开销） |
| `SDL_EventEnabled(type)` | 查询某类事件是否启用 |
| `SDL_AddEventWatch(callback)` | 注册回调，**任何事件**出队前先执行 |
| `SDL_RemoveEventWatch(callback)` | 移除回调 |

---

## 参考

- [SDL3 Events 官方文档](https://wiki.libsdl.org/SDL3/CategoryEvents)
- [SDL_EventType 枚举](https://wiki.libsdl.org/SDL3/SDL_EventType)
- [SDL_Event 联合体](https://wiki.libsdl.org/SDL3/SDL_Event)
