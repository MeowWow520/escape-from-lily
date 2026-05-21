## TODO 

### 对项目
- ✨ 实现 Background 类
- ❌ 实现 Player 类
- ❌ 实现 SceneTitle 类
- ❌ 实现 UserInterface(UI) 类
- ❌





### 对仓库
- ❌ 添加 `Readme.md` 中对 **其他平台** 构建方法的支持
- ❌ 完成 [Build.md](Build.md)
- ❌ 完成对 [Class.docs.md](Class.docs.md) 的编辑

### Bugs
- **BUG-1 [关键]**: `Game.cpp:66-67` — `else` 关键字缺失，`m_delta_time` 在 `if` 块内被赋值后立刻被第 67 行无条件覆盖
- **BUG-2 [关键]**: `Game.cpp:23` + `Game.h:67` — `m_frame_delay` 从未从 `m_FPS` 计算（始终为 0），帧率限制是死代码；且 `m_frame_delay` 应为 `Uint64` 以匹配 `SDL_DelayNS`
- **BUG-3 [关键]**: `Background.cpp:37-38` — `SDL_DestroyTexture` 后 `m_texture` 未置 `nullptr`，`Quit()` 被调用两次时导致 double-free UB
- **BUG-4 [关键]**: `CMakeLists.txt:70-77` — `spdlog` 未包含在 `target_link_libraries` 中，代码使用了 `spdlog::get()`、`spdlog::drop()` 等非头文件函数，会引起链接错误
- **BUG-5 [中]**: `Camera.cpp:9-10` — `Camera::Initialize()` 未调用 `MovableEntity::Initialize()`，`m_acceleration` 未被设置（当 `SWITCHER_ACCELERATION=true` 时），且违反生命周期约定
- **BUG-6 [中]**: `EntityFactory.cpp:18-22` — 相机工厂方法中变量名 `bg`（Background 拷贝残留），并存在多余的 `move` 到第二个变量
- **BUG-7 [中]**: `EntityFactory.cpp:17` — 结构化绑定 `const auto&[world_pos, m_border]` 中 `m_border` 遮蔽了 `Camera::m_border` 成员变量，易混淆
- **BUG-8 [中]**: `SceneMain.cpp:17-18` — `CameraParams camera = {...}` 聚合初始化遗漏 `m_border`，被零初始化后立即被 `Camera::Initialize()` 覆盖
- **BUG-9 [低]**: `Input.cpp:16-30` — `GetActionState()`/`IsActionPressed()` 等函数中 `find()->second` 未检查 `end()` 迭代器，查询未绑定动作时触发 UB
- **BUG-10 [低]**: `Camera.h:13` — `glm::vec2 m_camera_pos{}` 声明但从未使用，实际使用 `ObjectWorld::m_world_pos`
- **BUG-11 [低]**: `Camera.cpp:51-58` — `CanCameraActive()` 是私有方法且从未被调用
- **BUG-12 [低]**: `Game.cpp:56-60` — 主循环第一帧 `m_delta_time = 0`，`Update()` 使用零 delta time
