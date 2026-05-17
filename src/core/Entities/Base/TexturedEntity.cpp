//
// Created by MeowWow520 on 2026/5/10.
//

#include "TexturedEntity.h"
#include <SDL3_image/SDL_image.h>

#include "../../Scene.h"


TexturedEntity::TexturedEntity() = default;

using TexturePtr = std::unique_ptr<
                   SDL_Texture,
                   decltype(&SDL_DestroyTexture)
                   >;
TexturePtr TexturedEntity::SetTexture(TexturePtr newtexture) noexcept {
    m_texture = std::move(newtexture);
    return newtexture;
}

[[nodiscard]] SDL_Texture* TexturedEntity::GetTexture() const {
    return m_texture.get();
}

SDL_Color TexturedEntity::SetColor(const SDL_Color newcolor) {
    m_color_mod = newcolor;
    return newcolor;
}

SDL_Color TexturedEntity::GetColorMod() const {
    return m_color_mod;
}

SDL_BlendMode TexturedEntity::SetBlendMode(const SDL_BlendMode newblendmode) {
    m_blend_mode = newblendmode;
    return newblendmode;
}

SDL_BlendMode TexturedEntity::GetBlendMode() const {
    return m_blend_mode;
}

bool TexturedEntity::SetDirty(const bool newdirty) {
    m_texture_dirty = newdirty;
    return newdirty;
}

bool TexturedEntity::GetDirty() const {
    return m_texture_dirty;
}

float TexturedEntity::SetRotation(const float rotation) {
    m_rotation = rotation;
    return rotation;
}

[[nodiscard]] float TexturedEntity::GetRotation() const {
    return m_rotation;
}

glm::vec2 TexturedEntity::SetScale(const glm::vec2 newscale) {
    m_scale = newscale;
    return newscale;
}

[[nodiscard]] glm::vec2 TexturedEntity::GetScale() const {
    return m_scale;
}

glm::vec2 TexturedEntity::SetTextureSize(const glm::vec2 newtexturesize) {
    m_texture_size = newtexturesize;
    return newtexturesize;
}

glm::vec2 TexturedEntity::GetTextureSize() const {
    return m_texture_size;
}

glm::vec2 TexturedEntity::SetPivot(const glm::vec2 newpivot) {
    m_pivot = newpivot;
    return newpivot;
}

[[nodiscard]] glm::vec2 TexturedEntity::GetPivot() const {
    return m_pivot;
}

glm::vec2 TexturedEntity::SetHitbox(const glm::vec2 newhitbox) {
    m_entity_hitbox = newhitbox;
    return newhitbox;
}

glm::vec2 TexturedEntity::GetHitbox() const {
    return m_entity_hitbox;
}

bool TexturedEntity::SetVisible(const bool newvisible) {
    m_visible = newvisible;
    return newvisible;
}

[[nodiscard]] bool TexturedEntity::GetVisible() const {
    return m_visible;
}

std::string TexturedEntity::SetPath(std::string newpath) {
    m_path = newpath;
    return newpath;
}

[[nodiscard]] std::string TexturedEntity::GetPath() const {
    return m_path;
}

SDL_Rect TexturedEntity::SetRect(const SDL_Rect newrect) {
    m_rect = newrect;
    return newrect;
}

SDL_Rect TexturedEntity::GetRect() const {
    return m_rect;
}

bool TexturedEntity::InitializeTextureFromPath() {
    m_visible = true;
    if (SDL_LibInitChecker(!(!m_path.data() || m_path[0] == '\0'), "TexturedEntity::InitializeTextureFromPath"))
        return false;
    m_texture.reset();
    if (!SetTextureFromPath())
        return false;
    return true;
}

bool TexturedEntity::SetTextureFromPath() {
    // 创建新纹理
    SDL_Texture* newTexture = IMG_LoadTexture(m_game_instance.GetSDLRenderer(), m_path.c_str());
    if (SDL_LibInitChecker(newTexture != nullptr, "IMG_LoadTexture"))
        return false;
    // 设置新纹理 + 更新变量
    m_texture.reset(newTexture);
    SDL_GetTextureSize(m_texture.get(),&m_texture_size.x,&m_texture_size.y);
    return true;
}
// FIXME: 疑似无效？
glm::vec2 TexturedEntity::TransScreenPos() const {
    return m_world_pos - m_game_instance.GetCurrentScene()->GetCamera()->GetWorldPos();
}

// FIXME: 疑似逻辑错误？
bool TexturedEntity::IsInCameraRange() const {
    if (m_world_pos.x < 0 ||
        m_world_pos.y < 0 ||
        m_world_pos.x > m_game_instance.GetWindowSize().x ||
        m_world_pos.y > m_game_instance.GetWindowSize().y) {
        return false;
    } return true;
}
