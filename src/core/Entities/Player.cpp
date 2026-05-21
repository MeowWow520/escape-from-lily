//
// Created by MeowWow520 on 2026/5/17.
//

#include "Player.h"

#include <algorithm>
#include <SDL3_image/SDL_image.h>

#include "../Scene.h"
#include "../Logger/Log.h"


int Player::Initialize() {
    m_visible = true;
    if (!m_path.data() || m_path[0] == '\0') {
        EFL_LOGGER_ERROR(LogCategory::Entity, "Player Initialize() failed: m_path is NULL");
        return -1;
    }
    m_texture = IMG_LoadTexture(m_game_instance.GetSDLRenderer(), m_path.c_str());
    EFL_CHECK(LogCategory::Entity, m_texture != nullptr, "Player IMG_LoadTexture");
    SDL_GetTextureSize(m_texture, &m_texture_size.x, &m_texture_size.y);
    m_world_pos = (m_game_instance.GetCurrentScene()->GetWorldSize() - m_texture_size ) / glm::vec2(2.0f);
    EFL_LOGGER_INFO(LogCategory::Entity, "Play's m_world_pos be set in ({}, {})", m_world_pos.x, m_world_pos.y);
    m_screen_pos = TransScreenPos();
    return MovableEntity::Initialize();
}

void Player::HandleEvents(SDL_Event event) {
    MovableEntity::HandleEvents(event);
}

void Player::Update(const float dt) {
    m_world_pos += m_game_instance.GetKeyboardInput()->GetMovementNormalizeVec2() * m_max_speed * dt;
    m_world_pos.x = std::clamp(m_world_pos.x,0.0f,
        m_game_instance.GetCurrentScene()->GetWorldSize().x - m_texture_size.x);
    m_world_pos.y = std::clamp(m_world_pos.y, 0.0f,
        m_game_instance.GetCurrentScene()->GetWorldSize().y - m_texture_size.y);
    m_screen_pos = TransScreenPos();
}

void Player::Render() {
    const SDL_FRect destination = EFL_Vec2AddToRectFloat(m_screen_pos, m_texture_size);
    // 依据移动方向选择纹理样式
    if (m_game_instance.GetKeyboardInput()->GetMovementNormalizeVec2().x < 0.0f) {
        SDL_RenderTextureRotated(m_game_instance.GetSDLRenderer(), m_texture,
            nullptr, &destination,
            m_rotation, nullptr , SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTextureRotated(m_game_instance.GetSDLRenderer(), m_texture,
            nullptr, &destination,
            m_rotation, nullptr, SDL_FLIP_NONE);
    }
}

int Player::Quit() {
    if (m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    return MovableEntity::Quit();
}

std::string Player::SetName(std::string name) {
    m_player_name = name;
    return name;
}
