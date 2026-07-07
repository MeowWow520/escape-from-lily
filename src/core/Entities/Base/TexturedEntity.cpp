//
// Created by MeowWow520 on 2026/5/10.
//

#include "TexturedEntity.h"
#include <SDL3_image/SDL_image.h>

#include "../../Scene.h"



int TexturedEntity::Initialize() {
    return 0;
}

SDL_Texture * TexturedEntity::setTexture(SDL_Texture *new_texture) {
    m_texture = new_texture;
    return new_texture;
}


[[nodiscard]] SDL_Texture* TexturedEntity::getTexture() const {
    return m_texture;
}

SDL_Color TexturedEntity::setColor(const SDL_Color newcolor) {
    m_colorMod = newcolor;
    return newcolor;
}

SDL_Color TexturedEntity::getColorMod() const {
    return m_colorMod;
}

SDL_BlendMode TexturedEntity::setBlendMode(const SDL_BlendMode new_blend_mode) {
    m_blendMode = new_blend_mode;
    return new_blend_mode;
}

SDL_BlendMode TexturedEntity::getBlendMode() const {
    return m_blendMode;
}

bool TexturedEntity::setDirty(const bool new_dirty) {
    m_bTextureDirty = new_dirty;
    return new_dirty;
}

bool TexturedEntity::getDirty() const {
    return m_bTextureDirty;
}

float TexturedEntity::setRotation(const float rotation) {
    m_fRotation = rotation;
    return rotation;
}

[[nodiscard]] float TexturedEntity::getRotation() const {
    return m_fRotation;
}

glm::vec2 TexturedEntity::setScale(const glm::vec2 new_scale) {
    m_vec2_scale = new_scale;
    return new_scale;
}

[[nodiscard]] glm::vec2 TexturedEntity::getScale() const {
    return m_vec2_scale;
}

glm::vec2 TexturedEntity::setTextureSize(const glm::vec2 new_texture_size) {
    m_vec2_textureSize = new_texture_size;
    return new_texture_size;
}

glm::vec2 TexturedEntity::getTextureSize() const {
    return m_vec2_textureSize;
}

glm::vec2 TexturedEntity::setPivot(const glm::vec2 new_pivot) {
    m_vec2_pivot = new_pivot;
    return new_pivot;
}

[[nodiscard]] glm::vec2 TexturedEntity::getPivot() const {
    return m_vec2_pivot;
}

glm::vec2 TexturedEntity::setHitbox(const glm::vec2 new_hitbox) {
    m_vec2_entityHitbox = new_hitbox;
    return new_hitbox;
}

glm::vec2 TexturedEntity::getHitbox() const {
    return m_vec2_entityHitbox;
}

bool TexturedEntity::setVisible(const bool newvisible) {
    m_bVisible = newvisible;
    return newvisible;
}

[[nodiscard]] bool TexturedEntity::getVisible() const {
    return m_bVisible;
}

std::string TexturedEntity::setPath(std::string new_path) {
    m_strPath = new_path;
    return new_path;
}

[[nodiscard]] std::string TexturedEntity::getPath() const {
    return m_strPath;
}

SDL_Rect TexturedEntity::setRect(const SDL_Rect new_rect) {
    m_rect = new_rect;
    return new_rect;
}

SDL_Rect TexturedEntity::getRect() const {
    return m_rect;
}

glm::vec2 TexturedEntity::transScreenPos() const {
    return m_vec2_worldPos - m_game_instance.getCurrentScene()->GetCamera()->getWorldPos();
}