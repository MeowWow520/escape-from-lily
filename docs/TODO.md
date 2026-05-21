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
- **BUG-3 [关键]**: `Background.cpp:37-38` — `SDL_DestroyTexture` 后 `m_texture` 未置 `nullptr`，`Quit()` 被调用两次时导致 double-free UB
- **BUG-5 [中]**: `Camera.cpp:9-10` — `Camera::Initialize()` 未调用 `MovableEntity::Initialize()`，`m_acceleration` 未当被……设置 `SWITCHER_ACCELERATION=true` 时），且违反生命周期约定
- **BUG-7 [中]**: `EntityFactory.cpp:17` — 结构化绑定 `const auto&[world_pos, m_border]` 中 `m_border` 遮蔽了 `Camera::m_border` 成员变量，易混淆
- **BUG-8 [中]**: `SceneMain.cpp:17-18` — `CameraParams camera = {...}` 聚合初始化遗漏 `m_border`，被零初始化后立即被 `Camera::Initialize()` 覆盖
