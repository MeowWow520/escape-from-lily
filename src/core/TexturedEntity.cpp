//
// Created by MeowWow520 on 2026/5/10.
//

#include "TexturedEntity.h"

#include <SDL3_image/SDL_image.h>


TexturedEntity::TexturedEntity() { }

float TexturedEntity::SetRotation(const float rotation) {
    m_rotation = rotation;
    return rotation;
}

float TexturedEntity::GetRotation() const {
    return m_rotation;
}

glm::vec2 TexturedEntity::SetScale(const glm::vec2 newscale) {
    m_scale = newscale;
    return newscale;
}

glm::vec2 TexturedEntity::GetScale() const {
    return m_scale;
}

glm::vec2 TexturedEntity::SetPivot(const glm::vec2 newpivot) {
    m_pivot = newpivot;
    return newpivot;
}

glm::vec2 TexturedEntity::GetPivot() const {
    return m_pivot;
}

bool TexturedEntity::SetTextureFromPath(const char *path) {
    if (!path || path[0] == '\0') {
        SDL_Log("TexturedEntity::SetTextureFromPath failed: path is null");
        return false;
    }
    // 创建新纹理
    SDL_Texture* newTexture = IMG_LoadTexture(m_game_instance.GetSDLRenderer(), path);
    if (!newTexture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return false;
    }
    // 设置新纹理
    m_texture.reset(newTexture);
    // 更新变量
    SDL_GetTextureSize(m_texture.get(),&m_texture_size.x,&m_texture_size.y);
    return true;
}
