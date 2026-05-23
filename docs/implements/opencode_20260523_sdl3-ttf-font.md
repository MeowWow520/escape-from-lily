# SDL3_ttf 字体渲染实践与 FontManager 设计方案

> 设计日期：2026/5/23
> 来源：OpenCode 会话规划，基于 SDL3_ttf 3.0 官方文档

## 设计决策

| 决策 | 选择 | 理由 |
|------|------|------|
| 渲染路径 | **新 Text Engine**（`TTF_CreateRendererTextEngine` + `TTF_CreateText` + `TTF_DrawRendererText`） | SDL3_ttf 3.0 原生内置纹理 atlas，修改文字无需重建纹理，性能更好；`TTF_SetTextString` 支持运行时高效更新 |
| FontManager 归属 | **Scene 持有** | 每个 Scene 独立管理字体，不同场景可用不同字体集；生命周期与 Scene 绑定 |
| TextLabel 实体 | **一并设计，继承 ObjectScreen** | 提供可复用的 UI 文本显示实体，场景中即插即用，与现有 `UserInterface` 同级 |
| 字体缓存 key | `"filepath:ptsize"` | 同一字体文件不同字号视为独立缓存项，避免重复调用 `TTF_OpenFont` |
| 文本更新 | **`TTF_SetTextString` 原地更新** | Text Engine 内置纹理 atlas，修改文字时无需销毁/重建 `TTF_Text` 对象 |
| 日志分类 | **新增 `LogCategory::Font`** | 字体加载/缓存/错误日志统一管理，遵循现有 Logger 体系 |

## 项目现状

本项目已具备 SDL3_ttf 的基础条件，无需额外依赖：

- **`vcpkg.json`** — 已包含 `sdl3-ttf`
- **`CMakeLists.txt`** — 已包含 `find_package(SDL3_ttf CONFIG REQUIRED)` 和 `SDL3_ttf::SDL3_ttf`
- **`Game::Initialize()`** — 已调用 `TTF_Init()`，通过 `EFL_CHECK` 验证成功
- **`Game::Quit()`** — 末尾调用 `TTF_Quit()`

缺少的环节：
- 无字体文件（`assets/fonts/` 目录已空）
- 无字体缓存/管理机制
- 无文本显示实体

## SDL3_ttf 字体渲染基础

### 初始化与销毁

已在 `Game::Initialize()` 和 `Game::Quit()` 中完成：

```cpp
// Game::Initialize()
EFL_CHECK(LogCategory::Core, TTF_Init(), "TTF_Init");

// Game::Quit()
TTF_Quit();
```

### 打开与关闭字体

```cpp
TTF_Font* font = TTF_OpenFont("assets/fonts/NotoSansSC-Regular.ttf", 24.0f);
if (!font) {
    EFL_LOGGER_ERROR(LogCategory::Font, "Failed to load font: {}", SDL_GetError());
    return -1;
}
// ... 使用字体 ...
TTF_CloseFont(font);
```

`TTF_OpenFont` 第二个参数为**点大小**（ptsize），float 类型，支持非整数尺寸。

### 经典渲染路径（Surface → Texture）

SDL3_ttf 的传统用法——将文字渲染到 `SDL_Surface`，再转为 `SDL_Texture`：

```cpp
// 1. 渲染文字到 surface
SDL_Surface* surface = TTF_RenderText_Blended(font, "Hello World", 0, {255, 255, 255, 255});
if (!surface) { /* 错误处理 */ }

// 2. surface 转 texture
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

// 3. 释放 surface
SDL_DestroySurface(surface);

// 4. 使用 texture 渲染（SDL_RenderTexture）
SDL_FRect dst = {x, y, (float)surface->w, (float)surface->h};
SDL_RenderTexture(renderer, texture, nullptr, &dst);

// 5. 用完销毁 texture
SDL_DestroyTexture(texture);
```

**特点**：
- 每次改文字需要重新走完整流程（surface → texture）
- 纹理由调用方管理，适合静态文字
- 可套入 `TexturedEntity` 的 `m_texture` 体系

**渲染质量变体**：

| 函数 | 质量 | 性能 | 适用场景 |
|------|------|------|---------|
| `TTF_RenderText_Solid` | 最低（无抗锯齿） | 最快 | 低配设备、调试文字 |
| `TTF_RenderText_Shaded` | 中等（带背景色） | 中等 | 控制台风格 UI |
| `TTF_RenderText_Blended` | 高（抗锯齿 + 阿尔法混合） | 较慢 | **推荐**：通用文字渲染 |
| `TTF_RenderText_LCD` | 最高（子像素渲染） | 最慢 | LCD 屏幕专用 |

所有变体都有 `_Wrapped` 后缀版本（自动换行）：
```cpp
TTF_RenderText_Blended_Wrapped(font, "long text...", 0, color, wrap_width);
```

### 新 Text Engine 路径（推荐，本项目选用）

SDL3_ttf 3.0 引入了全新的 Text Engine API，内部管理纹理 atlas：

```
TTF_CreateRendererTextEngine(renderer)
    │
    ▼
TTF_CreateText(engine, font, "text", length)
    │
    ├── TTF_SetTextString(text, "new text", len)  ← 原地改内容
    ├── TTF_SetTextColor(text, color)              ← 改颜色
    │
    ▼
TTF_DrawRendererText(renderer, text, x, y)        ← 渲染
    │
    ▼
TTF_DestroyText(text)                              ← 销毁
    │
    ▼
TTF_DestroyRendererTextEngine(engine)              ← 销毁引擎
```

**完整示例：**

```cpp
// 1. 创建 Text Engine（每个 Renderer 一个引擎，全局共享）
TTF_TextEngine* engine = TTF_CreateRendererTextEngine(renderer);
if (!engine) {
    EFL_LOGGER_ERROR(LogCategory::Font, "Create TextEngine failed: {}", SDL_GetError());
    return -1;
}

// 2. 打开字体
TTF_Font* font = TTF_OpenFont("assets/fonts/NotoSansSC-Regular.ttf", 24.0f);
if (!font) { /* 错误 */ }

// 3. 创建 Text 对象（绑定引擎 + 字体）
TTF_Text* text = TTF_CreateText(engine, font, "Hello World", 11);
if (!text) { /* 错误 */ }

// 4. 设置颜色（可选，默认白色）
TTF_SetTextColor(text, {255, 255, 255, 255});

// 5. 渲染
TTF_DrawRendererText(renderer, text, 100, 200);

// 6. 修改文字内容（无需重建 TTF_Text）
TTF_SetTextString(text, "新的文字内容", -1);  // -1 表示 null-terminated

// 7. 再次渲染
TTF_DrawRendererText(renderer, text, 100, 250);

// 8. 获取文字尺寸
int w, h;
TTF_GetTextSize(text, &w, &h);

// 9. 销毁
TTF_DestroyText(text);
TTF_CloseFont(font);         // 仅在所有使用该 font 的 TTF_Text 销毁后
TTF_DestroyRendererTextEngine(engine);
```

**关键点**：
- `TTF_CreateRendererTextEngine` — 每个 `SDL_Renderer` **只需创建一次**，共享给所有 Text 对象
- `TTF_Text` 与 `TTF_Font` 在创建时绑定，后续可通过 `TTF_SetTextFont` 更换字体
- `TTF_SetTextString` — 高效原地更新文字内容，无需销毁/重建
- `TTF_GetTextSize` — 获取渲染后的实际像素尺寸
- 文字渲染不支持旋转/缩放——如果需要，仍应使用经典路径（Surface → Texture）套入 `TexturedEntity`

### Text Engine 渲染模型对比

| 特性 | 经典路径 | 新 Text Engine |
|------|---------|---------------|
| 纹理管理 | 调用方管理 `SDL_Texture*` | 引擎内部管理 atlas |
| 修改文字 | 重建 surface → texture | `TTF_SetTextString` 原地更新 |
| 缩放/旋转 | `SDL_RenderTextureRotated` | 不支持（直接 `Draw` 无变换） |
| 多行 | `_Wrapped` 系列函数 | 不支持（需自行分割多行） |
| 颜色 | 渲染时固定，改色需重建 | `TTF_SetTextColor` 运行时设定 |
| 与 TexturedEntity 整合 | 可以直接用 | 不能直接套用，需单独 Render |

本项目初期使用 **新 Text Engine** 路径作为主要渲染方式。对于需要旋转/缩放/粒子特效的文字（如标题动画），可退回到经典路径。

## FontManager 类设计

### 归属与职责

`FontManager` 是一个**非 Object 的独立服务类**，由 Scene 持有，负责字体加载/缓存和 Text Engine 管理：

```
SceneMain
  ├─ m_font_manager (FontManager)      ← 新增
  ├─ m_current_background (Background)
  ├─ m_camera (Camera)
  └─ m_player (Player)
```

### 文件位置

```
src/
  └─ core/
       └─ Font/
            ├─ FontManager.h
            └─ FontManager.cpp
            └─ TextLabel.h             ← 与 FontManager 同目录（同属字体子系统）
            └─ TextLabel.cpp
```

### 类定义

```cpp
// FontManager.h
#ifndef ESCAPE_FROM_LILY_FONTMANAGER_H
#define ESCAPE_FROM_LILY_FONTMANAGER_H

#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

class FontManager {
public:
    FontManager() noexcept;
    ~FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    /**
     * 初始化 FontManager：创建文字引擎并注册默认字体
     * @param renderer SDL_Renderer（从 Game 获取）
     * @return 0 成功，-1 失败
     */
    int Initialize(SDL_Renderer* renderer);

    /**
     * 加载字体文件到缓存
     * @param filepath 字体文件路径（如 "assets/fonts/NotoSansSC-Regular.ttf"）
     * @param ptsize 点大小
     * @return TTF_Font*，失败返回 nullptr
     */
    TTF_Font* LoadFont(const std::string& filepath, float ptsize);

    /**
     * 从缓存获取已加载的字体
     * @param filepath 字体文件路径
     * @param ptsize 点大小
     * @return TTF_Font*，未缓存时返回 nullptr（不会自动加载）
     */
    [[nodiscard]] TTF_Font* GetFont(const std::string& filepath, float ptsize) const;

    /**
     * 从缓存卸载指定字体
     */
    void UnloadFont(const std::string& filepath, float ptsize);

    /**
     * 卸载所有字体 + 销毁所有 Text 对象
     */
    void UnloadAll();

    /**
     * 创建文字对象
     * @param font 已加载的 TTF_Font*
     * @param text 文字内容（UTF-8）
     * @param color 文字颜色
     * @return TTF_Text*，失败返回 nullptr
     */
    TTF_Text* CreateText(TTF_Font* font, const std::string& text, SDL_Color color);

    /**
     * 销毁文字对象
     */
    void DestroyText(TTF_Text* text);

    /**
     * 渲染文字到屏幕
     * @param renderer SDL_Renderer
     * @param text TTF_Text 对象
     * @param x 屏幕 X 坐标
     * @param y 屏幕 Y 坐标
     */
    void DrawText(SDL_Renderer* renderer, TTF_Text* text, float x, float y);

    /**
     * 获取文字渲染后的像素尺寸
     * @return (width, height)，TTF_Text 无效时返回 (0, 0)
     */
    [[nodiscard]] glm::ivec2 GetTextSize(TTF_Text* text) const;

    /**
     * 清理所有资源
     * @return 0 成功
     */
    int Quit();

    // ── 便捷 Getter ──

    [[nodiscard]] TTF_TextEngine* GetTextEngine() const { return m_text_engine; }

private:
    /// 生成缓存 key: "filepath:ptsize"
    static std::string MakeKey(const std::string& filepath, float ptsize);

    TTF_TextEngine* m_text_engine{};
    std::unordered_map<std::string, TTF_Font*> m_font_cache;
};

#endif //ESCAPE_FROM_LILY_FONTMANAGER_H
```

### 核心实现

```cpp
// FontManager.cpp
#include "FontManager.h"
#include "../Logger/Log.h"

FontManager::FontManager() noexcept {
    EFL_LOG_ENTITY_CREATED("FontManager");
}

FontManager::~FontManager() {
    EFL_LOG_ENTITY_DESTROYED("FontManager");
}

int FontManager::Initialize(SDL_Renderer* renderer) {
    m_text_engine = TTF_CreateRendererTextEngine(renderer);
    EFL_CHECK(LogCategory::Font, m_text_engine != nullptr, "TTF_CreateRendererTextEngine");
    EFL_LOGGER_INFO(LogCategory::Font, "FontManager initialized");
    return 0;
}

TTF_Font* FontManager::LoadFont(const std::string& filepath, float ptsize) {
    const std::string key = MakeKey(filepath, ptsize);

    // 如果已缓存，直接返回
    if (const auto it = m_font_cache.find(key); it != m_font_cache.end()) {
        EFL_LOGGER_INFO(LogCategory::Font, "Font cache hit: {}", key);
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(filepath.c_str(), ptsize);
    if (!font) {
        EFL_LOGGER_ERROR(LogCategory::Font, "TTF_OpenFont failed: {} - {}", filepath, SDL_GetError());
        return nullptr;
    }

    m_font_cache[key] = font;
    EFL_LOGGER_INFO(LogCategory::Font, "Font loaded: {} ({}pt)", filepath, ptsize);
    return font;
}

TTF_Font* FontManager::GetFont(const std::string& filepath, const float ptsize) const {
    const std::string key = MakeKey(filepath, ptsize);
    if (const auto it = m_font_cache.find(key); it != m_font_cache.end()) {
        return it->second;
    }
    return nullptr;
}

void FontManager::UnloadFont(const std::string& filepath, const float ptsize) {
    const std::string key = MakeKey(filepath, ptsize);
    if (const auto it = m_font_cache.find(key); it != m_font_cache.end()) {
        TTF_CloseFont(it->second);
        m_font_cache.erase(it);
        EFL_LOGGER_INFO(LogCategory::Font, "Font unloaded: {}", key);
    }
}

void FontManager::UnloadAll() {
    for (const auto& [key, font] : m_font_cache) {
        TTF_CloseFont(font);
    }
    m_font_cache.clear();
    EFL_LOGGER_INFO(LogCategory::Font, "All fonts unloaded");
}

TTF_Text* FontManager::CreateText(TTF_Font* font, const std::string& text, const SDL_Color color) {
    if (!m_text_engine) {
        EFL_LOGGER_ERROR(LogCategory::Font, "TextEngine not initialized");
        return nullptr;
    }
    if (!font) {
        EFL_LOGGER_ERROR(LogCategory::Font, "CreateText: font is null");
        return nullptr;
    }

    TTF_Text* ttf_text = TTF_CreateText(m_text_engine, font, text.c_str(), text.length());
    if (!ttf_text) {
        EFL_LOGGER_ERROR(LogCategory::Font, "TTF_CreateText failed: {}", SDL_GetError());
        return nullptr;
    }
    TTF_SetTextColor(ttf_text, color);
    return ttf_text;
}

void FontManager::DestroyText(TTF_Text* text) {
    if (text) {
        TTF_DestroyText(text);
    }
}

void FontManager::DrawText(SDL_Renderer* renderer, TTF_Text* text, const float x, const float y) {
    if (!text || !renderer) return;
    TTF_DrawRendererText(renderer, text, static_cast<int>(x), static_cast<int>(y));
}

glm::ivec2 FontManager::GetTextSize(TTF_Text* text) const {
    if (!text) return {0, 0};
    int w, h;
    TTF_GetTextSize(text, &w, &h);
    return {w, h};
}

int FontManager::Quit() {
    UnloadAll();
    if (m_text_engine) {
        TTF_DestroyRendererTextEngine(m_text_engine);
        m_text_engine = nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Font, "FontManager quit");
    return 0;
}

std::string FontManager::MakeKey(const std::string& filepath, const float ptsize) {
    return filepath + ":" + std::to_string(static_cast<int>(ptsize));
}
```

**设计要点**：

- `LoadFont` — 先查缓存，已加载直接返回；未加载则 `TTF_OpenFont` 并存入 `m_font_cache`
- `CreateText` — 依赖 `m_text_engine` 已初始化，返回的 `TTF_Text*` 由调用方（通常是 `TextLabel`）在生命周期中保存
- `DestroyText` — 仅销毁 `TTF_Text`，不关闭字体（字体由 `UnloadFont`/`UnloadAll` 管理）
- `Quit` — 先 `UnloadAll` 关闭所有字体，再销毁 Text Engine

## TextLabel 实体设计

### 设计思路

`TextLabel` 继承自 `ObjectScreen`（而非 `TexturedEntity`），因为：

1. 新 Text Engine 不产生 `SDL_Texture*`，无法填入 TexturedEntity 的 `m_texture` 管道
2. 渲染走 `TTF_DrawRendererText`，与 `SDLRenderTexture` 流程不同
3. 屏幕坐标 (`m_screen_pos`) 恰好由 `ObjectScreen` 提供

```
Object → ObjectWorld (m_world_pos) → ObjectScreen (m_screen_pos) → TextLabel
```

### 类定义

```cpp
// TextLabel.h
#ifndef ESCAPE_FROM_LILY_TEXTLABEL_H
#define ESCAPE_FROM_LILY_TEXTLABEL_H

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Object/ObjectScreen.h"

class FontManager;

class TextLabel : public ObjectScreen {
public:
    explicit TextLabel(const char* entity_name = "TextLabel")
        : ObjectScreen(entity_name) {}

    ~TextLabel() override = default;

    int Initialize() override;
    void Render() override;
    int Quit() override;

    /// 设置文字内容（UTF-8），调用 TTF_SetTextString
    void SetText(const std::string& new_text);

    /// 设置文字颜色
    void SetColor(SDL_Color new_color);

    /// 设置字体（从 FontManager 加载后传入）
    void SetFont(TTF_Font* font);

    /// 获取渲染尺寸
    [[nodiscard]] glm::ivec2 GetTextSize() const;

    // ── Getter ──
    [[nodiscard]] const std::string& GetText() const { return m_content; }
    [[nodiscard]] SDL_Color GetColor() const { return m_color; }
    [[nodiscard]] TTF_Font* GetFont() const { return m_font; }

private:
    FontManager* m_font_manager{};       // 来自 Scene 的 FontManager
    TTF_Font*    m_font{};               // 绑定的字体对象
    TTF_Text*    m_text{};               // Text Engine 文字对象
    std::string  m_content;              // 当前文字内容
    SDL_Color    m_color{255, 255, 255, 255}; // 文字颜色
};

#endif //ESCAPE_FROM_LILY_TEXTLABEL_H
```

### TextLabel.cpp 实现示范

```cpp
// TextLabel.cpp
#include "TextLabel.h"
#include "FontManager.h"
#include "../Logger/Log.h"

int TextLabel::Initialize() {
    // FontManager 通过外部注入的方式获取（推荐）
    // 也可通过 m_game_instance 间接获取
    if (!m_font || !m_font_manager) {
        EFL_LOGGER_ERROR(LogCategory::Font, "TextLabel: font or font_manager not set");
        return -1;
    }

    if (m_text) {
        TTF_DestroyText(m_text);
        m_text = nullptr;
    }

    m_text = m_font_manager->CreateText(m_font, m_content, m_color);
    EFL_CHECK(LogCategory::Font, m_text != nullptr, "TextLabel CreateText");

    return 0;
}

void TextLabel::SetText(const std::string& new_text) {
    m_content = new_text;
    if (m_text) {
        TTF_SetTextString(m_text, m_content.c_str(), m_content.length());
    }
}

void TextLabel::SetColor(const SDL_Color new_color) {
    m_color = new_color;
    if (m_text) {
        TTF_SetTextColor(m_text, m_color);
    }
}

void TextLabel::SetFont(TTF_Font* font) {
    m_font = font;
    // 如果已有 TTF_Text，需要重建
    if (m_text && m_font_manager) {
        TTF_DestroyText(m_text);
        m_text = m_font_manager->CreateText(m_font, m_content, m_color);
    }
}

void TextLabel::Render() {
    if (!m_visible || !m_text) return;

    SDL_Renderer* renderer = m_game_instance.GetSDLRenderer();
    m_font_manager->DrawText(renderer, m_text, m_screen_pos.x, m_screen_pos.y);
}

glm::ivec2 TextLabel::GetTextSize() const {
    return m_font_manager ? m_font_manager->GetTextSize(m_text) : glm::ivec2{0, 0};
}

int TextLabel::Quit() {
    if (m_text && m_font_manager) {
        m_font_manager->DestroyText(m_text);
        m_text = nullptr;
    }
    m_font = nullptr;  // 字体由 FontManager 管理，不在此关闭
    return 0;
}
```

## 集成到现有架构

### 新增 LogCategory::Font

在 `src/core/Logger/LogCategory.h` 中添加：

```cpp
enum class LogCategory {
    Core,
    Entity,
    Input,
    Scene,
    Renderer,
    Factory,
    Font        // ← 新增
};
```

在 `src/core/Logger/Log.cpp` 的 `GetCategoryName` 和 `RegisterLogCategory` 中同步添加：

```cpp
// GetCategoryName
case LogCategory::Font: return "Font";

// RegisterLogCategory
CreateLogger("Font", config, file_sink);

// QuitLogger
spdlog::get("Font")->flush();
spdlog::drop("Font");
```

### CMakeLists.txt

在 `add_executable` 中添加：

```cmake
src/core/Font/FontManager.cpp
src/core/Font/FontManager.h
src/core/Font/TextLabel.cpp
src/core/Font/TextLabel.h
```

`SDL3_ttf::SDL3_ttf` 已在 `target_link_libraries` 中，无需修改。

### SceneMain 集成示例

```cpp
// SceneMain.h — 新增成员
#include "core/Font/FontManager.h"
#include "core/Font/TextLabel.h"

class SceneMain : public Scene {
    // ... 现有成员 ...
private:
    FontManager m_font_manager;
    EntityPtr<TextLabel> m_title_label;
};
```

```cpp
// SceneMain::Initialize() — 新增字体初始化
int SceneMain::Initialize() {
    // ... 现有初始化（世界缩放、世界大小）...

    // 初始化 FontManager
    EFL_CHECK(LogCategory::Font,
        m_font_manager.Initialize(m_game_instance.GetSDLRenderer()) == 0,
        "FontManager Initialize");

    // 加载字体
    TTF_Font* font = m_font_manager.LoadFont("assets/fonts/NotoSansSC-Regular.ttf", 36.0f);
    EFL_CHECK(LogCategory::Font, font != nullptr, "LoadFont NotoSansSC");

    // 创建 TextLabel
    m_title_label = std::make_unique<TextLabel>();
    m_title_label->SetFont(font);
    m_title_label->SetText("Escape from Lily");
    m_title_label->SetColor({255, 255, 255, 255});  // 白色
    m_title_label->SetScreenPos({100, 50});
    EFL_CHECK(LogCategory::Font, m_title_label->Initialize() == 0, "TextLabel Initialize");

    return 0;
}
```

```cpp
// SceneMain::Update()
void SceneMain::Update(const float dt) {
    m_camera->Update(dt);
    m_current_background->Update(dt);
    m_player->Update(dt);

    // TextLabel 一般不需要 Update（除非有动画）
}

// SceneMain::Render()
void SceneMain::Render() {
    m_current_background->Render();
    m_player->Render();
    m_title_label->Render();         // ← 渲染文字在最上层
}

// SceneMain::Quit()
int SceneMain::Quit() {
    m_title_label.reset();           // EntityDeleter 自动调 Quit() + delete
    m_font_manager.Quit();           // 关闭所有字体 + Text Engine
    m_camera.reset();
    m_current_background.reset();
    m_player.reset();
    return 0;
}
```

## RichTextBase 基类设计（未来扩展）

### 设计动机

当需要**每个字独立样式**（如彩虹色标题、逐字飞入动画、部分文字加粗/变色）时，不能简单地存储一个纯字符串。但也不应退化为 N 个独立 `TextLabel`——那样会失去 Text Engine 的合批优势（单次 `DrawRendererText`）和字距排版。

`RichTextBase` 的核心理念：**字符串存一份，样式按段叠加，渲染时智能合并**。

### 类定义

```cpp
// RichTextBase.h
#ifndef ESCAPE_FROM_LILY_RICHTEXTBASE_H
#define ESCAPE_FROM_LILY_RICHTEXTBASE_H

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Object/ObjectScreen.h"

class FontManager;

/// 单个字符的样式覆盖
struct CharStyle {
    SDL_Color color{255, 255, 255, 255};
    TTF_Font* font{nullptr};       // nullptr = 使用默认字体
};

/// 连续区间的样式定义
struct TextStyleSpan {
    size_t start{};
    size_t length{};               // 0 表示「直到末尾」
    CharStyle style;
};

class RichTextBase : public ObjectScreen {
public:
    explicit RichTextBase(const char* entity_name = "RichTextBase")
        : ObjectScreen(entity_name) {}

    ~RichTextBase() override = default;

    int Initialize() override;
    void Render() override;
    int Quit() override;

    /// 设置完整文字内容
    void SetText(const std::string& text);

    /// 设置某段文字的样式（追加到 m_spans）
    void AddStyleSpan(size_t start, size_t length, CharStyle style);

    /// 清除所有样式覆盖
    void ClearStyleSpans();

    /// 标记脏，下次 Render 前重建 TTF_Text 分段
    void MarkDirty() { m_dirty = true; }

private:
    /// 根据 string + spans 生成若干连续 TTF_Text 分段
    void Rebuild();

    FontManager* m_font_manager{};
    TTF_Font*    m_default_font{};              // 默认字体
    std::string  m_text;                         // 完整文字内容
    std::vector<TextStyleSpan> m_spans;          // 样式覆盖（通常 1~5 个）
    std::vector<TTF_Text*> m_segments;           // 重建后的渲染分段
    bool m_dirty{true};                          // 需要重建
};

#endif //ESCAPE_FROM_LILY_RICHTEXTBASE_H
```

### 渲染决策树

```
Rebuild() 内部逻辑：

m_spans 为空
  └─ m_segments = { 1 个 TTF_Text } → 1 次 Draw

m_spans 非空，合并相邻同样式区间后：
  └─ m_segments 数量 = M（M ≤ 样式变化次数 + 1）
  └─ 例："Hello[红色]World[蓝色]!"
      → 3 个 TTF_Text → 3 次 Draw

m_spans 密集，每个 pos 都有独立样式
  └─ m_segments 数量 ≈ N（逐字，用于动画特效）
```

### Rebuild 实现核心

```cpp
void RichTextBase::Rebuild() {
    if (!m_dirty) return;

    // 清除旧分段
    for (auto* seg : m_segments) {
        m_font_manager->DestroyText(seg);
    }
    m_segments.clear();

    if (m_spans.empty()) {
        // 无样式覆盖 → 1 个 TTF_Text，最高效
        TTF_Text* text = m_font_manager->CreateText(
            m_default_font, m_text, {255, 255, 255, 255});
        if (text) m_segments.push_back(text);
        m_dirty = false;
        return;
    }

    // 构建「字符下标 → CharStyle」映射
    std::vector<CharStyle> char_styles(m_text.length());
    for (auto& style : char_styles) {
        style.color = {255, 255, 255, 255};
        style.font = m_default_font;
    }
    for (const auto& span : m_spans) {
        size_t end = span.length == 0 ? m_text.length()
                                      : span.start + span.length;
        for (size_t i = span.start; i < end && i < m_text.length(); i++) {
            char_styles[i] = span.style;
        }
    }

    // 合并相邻同样式字符为分段
    size_t seg_start = 0;
    CharStyle current = char_styles[0];

    for (size_t i = 1; i <= m_text.length(); i++) {
        if (i == m_text.length() ||
            char_styles[i].color.r != current.color.r ||
            char_styles[i].color.g != current.color.g ||
            char_styles[i].color.b != current.color.b ||
            char_styles[i].color.a != current.color.a ||
            char_styles[i].font != current.font) {

            // [seg_start, i) 是连续同样式区间
            std::string segment = m_text.substr(seg_start, i - seg_start);
            TTF_Text* seg_text = m_font_manager->CreateText(
                current.font ? current.font : m_default_font,
                segment, current.color);
            if (seg_text) m_segments.push_back(seg_text);

            if (i < m_text.length()) {
                seg_start = i;
                current = char_styles[i];
            }
        }
    }

    m_dirty = false;
}
```

### Render 实现

```cpp
void RichTextBase::Render() {
    if (!m_visible) return;

    if (m_dirty) Rebuild();

    SDL_Renderer* renderer = m_game_instance.GetSDLRenderer();
    float cursor_x = m_screen_pos.x;

    for (auto* seg : m_segments) {
        m_font_manager->DrawText(renderer, seg, cursor_x, m_screen_pos.y);
        glm::ivec2 size = m_font_manager->GetTextSize(seg);
        cursor_x += static_cast<float>(size.x);  // 手动拼接 x 偏移
    }
}
```

> **注意**：分段后字距 (Kerning) 和文字整形丢失。跨段的 `AV` 不会自动缩紧，`ﬁ` 合字不会跨越段边界。对于中文（无 kerning 需求）影响很小；对于英文，建议仅在需要视觉特效的场景使用分段。

### 使用示例

```cpp
// 彩虹标题：每个字符不同颜色
const SDL_Color rainbow[] = {
    {255, 0, 0, 255},     // 红
    {255, 165, 0, 255},   // 橙
    {255, 255, 0, 255},   // 黄
    {0,   255, 0, 255},   // 绿
    {0,   0,   255, 255}, // 蓝
    {128, 0,   128, 255}  // 紫
};

auto title = std::make_unique<RichTextBase>();
title->SetFontManager(&m_font_manager);
title->SetFont(default_font);
title->SetText("ESCAPE");
for (size_t i = 0; i < 6; i++) {
    title->AddStyleSpan(i, 1, {rainbow[i], nullptr});
}
title->SetScreenPos({100, 100});
title->Initialize();

// 渲染时自动合并 → 6 个 TTF_Text → 6 次 Draw
// 虽然比纯字符串多，但仅限于标题区域，开销可接受

// 清除样式后回到 1 次 Draw
title->ClearStyleSpans();
title->MarkDirty();
```

### 与 TextLabel 的关系

| | TextLabel | RichTextBase |
|---|---|---|
| 适合场景 | 按钮、标签、对话、FPS | 标题动画、特效文字、部分变色 |
| 性能 | 1 个 TTF_Text / 1 次 Draw | M 个 TTF_Text / M 次 Draw |
| Kerning | 自动保留 | 分界线丢失 |
| 使用复杂度 | 低 | 中 |

`RichTextBase` 继承自 `ObjectScreen`，与 `TextLabel` 平级，而非继承自 `TextLabel`。两者共用 `FontManager` 服务。

## 运行时文字更新示例

```cpp
// 运行时更换文字内容
m_title_label->SetText("第 1 关");
// 渲染时自动使用新内容

// 运行时更换颜色
m_title_label->SetColor({255, 0, 0, 255});  // 红色

// 获取文字尺寸用于布局
glm::ivec2 size = m_title_label->GetTextSize();
float centered_x = (1920.0f - size.x) / 2.0f;
m_title_label->SetScreenPos({centered_x, 50});
```

`TTF_SetTextString` 和 `TTF_SetTextColor` 在 Text Engine 内部直接操作 atlas，无需重建纹理。

## 字体文件获取建议

本项目需要字体文件放在 `assets/fonts/` 目录。以下是常见开源中文字体的推荐（**请自行下载并放置**）：

| 字体 | 许可 | 推荐用途 |
|------|------|---------|
| [Noto Sans SC](https://fonts.google.com/noto/specimen/Noto+Sans+SC) | SIL Open Font 1.1 | 正文 UI、多语言 |
| [Source Han Sans](https://github.com/adobe-fonts/source-han-sans) | SIL Open Font 1.1 | 标题、正文 |
| [ZCOOL QingKe HuangYou](https://fonts.google.com/specimen/ZCOOL+QingKe+HuangYou) | SIL Open Font 1.1 | 装饰性标题 |

示例文件：
```
assets/fonts/NotoSansSC-Regular.ttf
assets/fonts/NotoSansSC-Bold.ttf
```

## 日志输出效果

```
[13:00:00.001] [   Font] [info] FontManager object constructed
[13:00:00.010] [   Font] [info] FontManager initialized
[13:00:00.020] [   Font] [info] Font loaded: assets/fonts/NotoSansSC-Regular.ttf (36pt)
[13:00:00.025] [   Font] [info] TextLabel object constructed
[13:00:00.030] [   Font] [info] Font cache hit: assets/fonts/NotoSansSC-Regular.ttf:36
[13:00:00.035] [   Font] [info] TextLabel object destroyed
[13:00:00.040] [   Font] [info] FontManager object destroyed
```

## 实现步骤

1. 在 `LogCategory.h` 中新增 `Font` 枚举值
2. 在 `Log.cpp` 的 `GetCategoryName` 和 `RegisterLogCategory` / `QuitLogger` 中同步添加 `Font`
3. 创建 `src/core/Font/FontManager.h` — 类声明
4. 创建 `src/core/Font/FontManager.cpp` — 完整实现（含 `#include "../Logger/Log.h"`）
5. 创建 `src/core/Font/TextLabel.h` — 实体声明（前向声明 `FontManager`）
6. 创建 `src/core/Font/TextLabel.cpp` — 实体实现
7. 在 `CMakeLists.txt` 中注册 `FontManager.cpp` 和 `TextLabel.cpp`
8. 在 `SceneMain.h` 中添加 `#include "core/Font/FontManager.h"` 和 `#include "core/Font/TextLabel.h"`
9. 在 `SceneMain` 中添加 `FontManager m_font_manager` 成员和 `EntityPtr<TextLabel> m_title_label`
10. 在 `SceneMain::Initialize()` 中添加 FontManager 初始化和 TextLabel 创建
11. 在 `SceneMain::Render()` 中添加 `m_title_label->Render()`
12. 在 `SceneMain::Quit()` 中添加 `m_font_manager.Quit()` 和 `m_title_label.reset()`
13. 将字体文件放入 `assets/fonts/`
14. 构建验证

## 扩展路线（未来）

- **TextButton** — 继承 TextLabel，添加点击检测、悬停变色、背景框
- **TextFPS** — 在场景中实时显示 FPS（每帧调用 `SetText`）
- **多行排版** — 手动拆分 `\n` 创建多个 TextLabel，或用经典路径的 `_Wrapped` 变体
- **文字动画** — 经典路径 + TexturedEntity 实现旋转/缩放/淡入淡出的标题效果
- **RichTextBase 落地** — 实现完整的 `RichTextBase` 类（设计见上文），支持 `AddStyleSpan` 的富文本标记解析器（如 `[color=red]...[/color]` 语法）
- **字体回退（Fallback）** — `TTF_AddFallbackFont` 实现中文字体 → 日文字体 → emoji 字体逐级回退
- **SDF 字体** — `TTF_SetFontSDF(font, true)` 开启 SDF（Signed Distance Field）渲染，支持任意缩放不失真
- **配置驱动** — 从 JSON 读取字体路径和字号，运行时动态加载

## 经典路径回退指南

当需要旋转/缩放文字时，回退到经典路径（Surface → Texture）套入 `TexturedEntity`：

```cpp
// 在 FontManager 中增加便捷方法
SDL_Texture* FontManager::RenderTextToTexture(SDL_Renderer* renderer,
    TTF_Font* font, const std::string& text, SDL_Color color, int& out_w, int& out_h) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) {
        EFL_LOGGER_ERROR(LogCategory::Font, "RenderText_Blended failed: {}", SDL_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    out_w = surface->w;
    out_h = surface->h;
    SDL_DestroySurface(surface);
    return texture;
}
```

然后将返回的 `SDL_Texture*` 通过 `TexturedEntity::SetTexture()` 注入，即可使用旋转/缩放/颜色调制。

## 参考

- [SDL3_ttf 3.0 官方文档](https://wiki.libsdl.org/SDL3_ttf/FrontPage)
- [SDL3_ttf API 索引](https://wiki.libsdl.org/SDL3_ttf/CategoryAPI)
- [TTF_OpenFont](https://wiki.libsdl.org/SDL3_ttf/TTF_OpenFont) — 打开字体
- [TTF_CreateRendererTextEngine](https://wiki.libsdl.org/SDL3_ttf/TTF_CreateRendererTextEngine) — 创建文字引擎
- [TTF_CreateText](https://wiki.libsdl.org/SDL3_ttf/TTF_CreateText) — 创建文字对象
- [TTF_DrawRendererText](https://wiki.libsdl.org/SDL3_ttf/TTF_DrawRendererText) — 渲染文字
- [TTF_SetTextString](https://wiki.libsdl.org/SDL3_ttf/TTF_SetTextString) — 更新文字内容
- [TTF_RenderText_Blended](https://wiki.libsdl.org/SDL3_ttf/TTF_RenderText_Blended) — 经典路径渲染（Surface）
- [TTF_AddFallbackFont](https://wiki.libsdl.org/SDL3_ttf/TTF_AddFallbackFont) — 字体回退
- 项目现有继承链：`Object` → `ObjectWorld` → `ObjectScreen` → `TexturedEntity`（见 AGENTS.md）
- 项目现有生命周期：构造 → `Initialize()` → `Update()`/`Render()` → `Quit()` → 析构（见 `Object.h`）
- 项目现有日志模式：`EF_LOGGER_INFO/ERROR(cat, ...)`，`EFL_CHECK(cat, expr, msg)`（见 `src/core/Logger/Log.h`）
- `Game::Initialize()` 已调用 `TTF_Init()`，`Game::Quit()` 已调用 `TTF_Quit()`（见 `src/core/Game.cpp`）
