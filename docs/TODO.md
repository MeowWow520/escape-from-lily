# 重构计划 — 命名规范化

## 约定

| 类别 | 规范 |
|------|------|
| 类名 | PascalCase |
| 方法名 | PascalCase |
| 成员变量 | `m_` 前缀 + snake_case |
| 函数参数 | snake_case，setter 参数用 `new_xxx` |
| 枚举 (类型+值) | PascalCase |
| 宏 | `EFL_` 前缀 + UPPER_SNAKE_CASE |
| 文件名 | 与类名一致（PascalCase） |
| Include guard | `ESCAPE_FROM_LILY_<NAME>_H` |
| struct 成员 | 统一使用 `m_` 前缀 |
| 布尔查询方法 | `Is` 前缀（如 `IsActionPressed`） |

## Phase 1 — 宏重命名

- [x] `EFL_CHACK_WITH_GET_ERROR` → `EFL_CHECK_WITH_GET_ERROR`（Log.h + 所有调用点）
- [ ] `COLOR(HEX)` → `EFL_COLOR(HEX)`（Def.h + 所有调用点）
- [ ] `DEFAULT_FONT_PATH` → `EFL_DEFAULT_FONT_PATH`
- [ ] `DEFAULT_FONT_SIZE` → `EFL_DEFAULT_FONT_SIZE`
- [ ] `DEFAULT_TEXT_COLOR` → `EFL_DEFAULT_TEXT_COLOR`

## Phase 2 — 成员变量重命名

### ConfigManager
- [ ] `m_defaultJson` → `m_default_json`
- [ ] `m_playerJson` → `m_player_json`
- [ ] `m_fontJsonMap` → `m_font_json_map`

### Game
- [ ] `m_FPS` → `m_fps`

### HealthyPointEntity
- [ ] `m_f_health` → `m_health`
- [ ] `m_f_get_damage` → `m_get_damage`

### EntityTypes.h — struct 成员统一加 `m_` 前缀
- [ ] `PlayerParams::texture_path` → `PlayerParams::m_texture_path`
- [ ] `CameraParams::world_pos` → `CameraParams::m_world_pos`
- [ ] 检查其他参数字段是否遗漏

## Phase 3 — 方法重命名

### TextBase getter/setter 标准化
- [ ] `GetText_font_size()` → `GetFontSize()`
- [ ] `SetText_font_size()` → `SetFontSize()`
- [ ] `GetText_stype()` → `GetTextStyle()`
- [ ] `SetText_stype()` → `SetTextStyle()`
- [ ] `GetText_color()` → `GetTextColor()`
- [ ] `SetText_color()` → `SetTextColor()`

### TexturedEntity
- [ ] `TransScreenPos()` → `ToScreenPos()`（h + cpp + 调用点）
- [ ] `SetColor()` → `SetColorMod()`（h + cpp + 调用点）

## Phase 4 — 构造函数参数重命名

- [ ] `Object(const char* m_entity_name)` → `Object(const char* entity_name)`
- [ ] `ObjectWorld(const char* m_entity_name)` → `ObjectWorld(const char* entity_name)`
- [ ] `ObjectScreen(const char* m_entity_name)` → `ObjectScreen(const char* entity_name)`
- [ ] `TexturedEntity(const char* m_entity_name)` → `TexturedEntity(const char* entity_name)`
- [ ] `MovableEntity(const char* m_entity_name)` → `MovableEntity(const char* entity_name)`
- [ ] `TextBase(const char* m_entity_name)` → `TextBase(const char* entity_name)`
- [ ] `TextStatic(const char* m_entity_name)` → `TextStatic(const char* entity_name)`
- [ ] `HealthyPointEntity(const char* m_entity_name)` → `HealthyPointEntity(const char* entity_name)`
- [ ] Background
- [ ] Camera
- [ ] Player
- [ ] 各 .cpp 中修正 `m_entity_name(m_entity_name)` 为 `m_entity_name(entity_name)`

## Phase 5 — Setter 参数名规范化（TexturedEntity）

统一改为 `new_xxx` 蛇形命名：
- [ ] `SetTexture(SDL_Texture* newtexture)` → `SetTexture(SDL_Texture* new_texture)`
- [ ] `SetDirty(bool newdirty)` → `SetDirty(bool new_dirty)`
- [ ] `SetRotation(float rotation)` → `SetRotation(float new_rotation)`
- [ ] `SetScale(glm::vec2 newscale)` → `SetScale(glm::vec2 new_scale)`
- [ ] `SetPivot(glm::vec2 newpivot)` → `SetPivot(glm::vec2 new_pivot)`
- [ ] `SetPath(std::string newpath)` → `SetPath(std::string new_path)`
- [ ] `SetHitbox(glm::vec2 newhitbox)` → `SetHitbox(glm::vec2 new_hitbox)`
- [ ] `SetMaxSpeed(float newmaxspeed)` → `SetMaxSpeed(float new_max_speed)`
- [ ] `SetAcceleration(float newacceleration)` → `SetAcceleration(float new_acceleration)`
- [ ] `SetVector(glm::vec2 newvector)` → `SetVector(glm::vec2 new_vector)`
- [ ] `SetVisible(bool newvisible)` → `SetVisible(bool new_visible)`
- [ ] `SetBlendMode(SDL_BlendMode newblendmode)` → `SetBlendMode(SDL_BlendMode new_blend_mode)`
- [ ] 同步更新 .cpp 中的参数名

## Phase 6 — 文件 + 枚举重命名

- [ ] 在文件系统中重命名 `src/core/Def/EFL_FontDef.h` → `src/core/Def/FontDef.h`
- [ ] 更新 `FontDef.h` 的 include guard
- [ ] 更新所有 `#include "core/Def/EFL_FontDef.h"` → `#include "core/Def/FontDef.h"`
- [ ] 在文件系统中重命名 `src/core/Font/TextStypes.h` → `src/core/Font/TextStyles.h`
- [ ] 更新 `TextStyles.h` 的 include guard
- [ ] 更新所有 `#include "core/Font/TextStypes.h"` → `#include "core/Font/TextStyles.h"`
- [ ] `enum TextStype` → `enum TextStyle`（TextStyles.h + 所有引用）
- [ ] 更新 `CMakeLists.txt` 中的文件名引用（如有）

## Phase 7 — 文档更新

- [ ] `AGENTS.md`: 删除不存在的常量引用，更新命名规范描述
- [ ] `docs/CONTRIBUTING.md`: 修正变量命名规范，匹配实际代码风格
- [ ] 更新 `docs/Build.md`（如需要）
- [ ] 更新 `Readme.md`（如需要）

## 工作量统计

| Phase | 涉及文件 | 操作类型 | 预估 edit 数 |
|-------|---------|----------|-------------|
| 1 宏重命名 | ~12 | 批量 find-replace | ~30 |
| 2 成员变量 | ~6 | 精确匹配替换 | ~40 |
| 3 方法重命名 | ~10 | 逐点替换 | ~50 |
| 4 构造参数 | ~14 | 逐点替换 | ~20 |
| 5 Setter 参数 | ~2 | 逐点替换 | ~20 |
| 6 文件/枚举 | ~10 | 文件重命名 + find-replace | ~20 |
| 7 文档 | ~4 | 编辑 | ~10 |
| **总计** | **~40 个文件** | | **~190 edits** |

## 风险说明

- **High risk**: Phase 1、2、3 改动会破坏编译，漏改一处即编译失败。每个 rename 后建议立即编译验证。
- **Low risk**: Phase 4、5 仅改参数名（对外不可见），不影响编译。
- **Medium risk**: Phase 6 文件重命名需 OS 级操作 + 同步更新所有 `#include`。
- **No risk**: Phase 7 纯文档。
