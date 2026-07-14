//
// Created by MeowWow520 on 2026/5/17.
//

#include "Player.h"

#include <algorithm>
#include <SDL3_image/SDL_image.h>

#include "../Scene.h"
#include "../Logger/Log.h"


int Player::Initialize() {
    if (!m_strPath.data() || m_strPath[0] == '\0') {
        EFL_LOGGER_ERROR(LogCategory::Entity, "Player Initialize() failed: m_path is NULL");
        return -1;
    }
    m_texture = IMG_LoadTexture(m_game_instance.getSDLRenderer(), m_strPath.c_str());
    EFL_CHECK(LogCategory::Entity, m_texture, "Player IMG_LoadTexture");
    SDL_GetTextureSize(m_texture, &m_vec2_textureSize.x, &m_vec2_textureSize.y);
    m_vec2_worldPos = (m_game_instance.getCurrentScene()->GetWorldSize() - m_vec2_textureSize ) / glm::vec2(2.0f);
    EFL_LOGGER_INFO(LogCategory::Entity, "Play's m_world_pos be set in ({}, {})", m_vec2_worldPos.x, m_vec2_worldPos.y);
    m_vec2_screenPos = transScreenPos();
    return HealthyPointEntity::Initialize();
}

void Player::handleEvents(SDL_Event event) {
    HealthyPointEntity::handleEvents(event);
}

void Player::Update(const float dt) {
    m_vec2_worldPos += m_game_instance.getKeyboardInput()->GetMovementNormalizeVec2() * m_fMaxSpeed * dt;
    m_vec2_worldPos.x = std::clamp(m_vec2_worldPos.x,0.0f,
        m_game_instance.getCurrentScene()->GetWorldSize().x - m_vec2_textureSize.x);
    m_vec2_worldPos.y = std::clamp(m_vec2_worldPos.y, 0.0f,
        m_game_instance.getCurrentScene()->GetWorldSize().y - m_vec2_textureSize.y);
    m_vec2_screenPos = transScreenPos();
}

void Player::Render() {
    const SDL_FRect destination = EFL_Vec2AddToRectFloat(m_vec2_screenPos, m_vec2_textureSize);
    // 依据移动方向选择纹理样式
    if (m_game_instance.getKeyboardInput()->GetMovementNormalizeVec2().x < 0.0f) {
        SDL_RenderTextureRotated(m_game_instance.getSDLRenderer(), m_texture,
            nullptr, &destination,
            m_fRotation, nullptr , SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTextureRotated(m_game_instance.getSDLRenderer(), m_texture,
            nullptr, &destination,
            m_fRotation, nullptr, SDL_FLIP_NONE);
    }
}

int Player::Quit() {
    if (m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    return HealthyPointEntity::Quit();
}

std::string Player::setName(std::string name) {
    m_strPlayerName = name;
    return name;
}
