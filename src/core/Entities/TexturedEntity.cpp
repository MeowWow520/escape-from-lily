//
// Created by MeowWow520 on 2026/5/10.
//

#include "TexturedEntity.h"
#include <SDL3_image/SDL_image.h>

#include "../Scene.h"


TexturedEntity::TexturedEntity() = default;

using TexturePtr = std::unique_ptr<
                SDL_Texture,
                decltype(&SDL_DestroyTexture)>;
TexturePtr TexturedEntity::SetTexture(TexturePtr newtexture) noexcept {
    m_texture = std::move(newtexture);
    return newtexture;
}

[[nodiscard]] SDL_Texture* TexturedEntity::GetTexture() const {
    return m_texture.get();
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

glm::vec2 TexturedEntity::SetPivot(const glm::vec2 newpivot) {
    m_pivot = newpivot;
    return newpivot;
}

[[nodiscard]] glm::vec2 TexturedEntity::GetPivot() const {
    return m_pivot;
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

std::string TexturedEntity::GetPath() const {
    return m_path;
}

bool TexturedEntity::InitializeTextureFromPath() {
    if (!m_path.data() || m_path[0] == '\0') {
        // TODO: chore
        return false;
    }
    m_texture.reset();
    if (!SetTextureFromPath())
        return false;
    m_visible = true;
    return true;
}

bool TexturedEntity::SetTextureFromPath() {
    if (!m_path.data() || m_path[0] == '\0') {
        // TODO: chore
        return false;
    }
    // 创建新纹理
    SDL_Texture* newTexture = IMG_LoadTexture(m_game_instance.GetSDLRenderer(), m_path.c_str());
    if (!newTexture) {
        // TODO: chore
        return false;
    }
    // 设置新纹理
    m_texture.reset(newTexture);
    // 更新变量
    SDL_GetTextureSize(m_texture.get(),&m_texture_size.x,&m_texture_size.y);
    return true;
}

glm::vec2 TexturedEntity::TransScreenPos() const {
    return m_world_pos - m_game_instance.GetCurrentScene()->GetCameraPos();
}

bool TexturedEntity::IsInCameraRange() const {
    if (m_screen_pos.x < 0 ||
        m_screen_pos.y < 0 ||
        m_screen_pos.x > m_game_instance.GetWindowSize().x ||
        m_screen_pos.y > m_game_instance.GetWindowSize().y) {
        return false;
    } return true;
}
