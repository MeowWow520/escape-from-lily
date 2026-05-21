//
// Created by MeowWow520 on 2026/5/17.
//

#include "Camera.h"
#include "../Scene.h"
#include <algorithm>


int Camera::Initialize() {
    m_visible = false;
    m_border = 100.0f * m_game_instance.GetCurrentScene()->GetWorldScale().x;
    m_camera_active_range= { 0, 0,
        m_game_instance.GetCurrentScene()->GetWorldSize().x - m_game_instance.GetWindowSize().x,
        m_game_instance.GetCurrentScene()->GetWorldSize().y - m_game_instance.GetWindowSize().y
    };

    // 设置碰撞箱
    const SDL_FRect window = {0, 0,
        m_game_instance.GetWindowSize().x,
        m_game_instance.GetWindowSize().y
    };
    // FIXME: 无用的变量？
    SDL_FRect hitbox_size = {0, 0, 0, 0};
    SDL_GetRectIntersectionFloat(&window,&m_camera_active_range, &hitbox_size);
    m_entity_hitbox = {hitbox_size.w, hitbox_size.h};

    return 0;
}

void Camera::Update(const float dt) {
    m_world_pos += m_game_instance.GetKeyboardInput()->GetMovementNormalizeVec2() * m_max_speed * dt;
    // 钳制到有效范围，确保相机始终在边界内且可以往回移动
    m_world_pos.x = std::clamp(m_world_pos.x, m_camera_active_range.x, m_camera_active_range.x + m_camera_active_range.w);
    m_world_pos.y = std::clamp(m_world_pos.y, m_camera_active_range.y, m_camera_active_range.y + m_camera_active_range.h);
}


SDL_FRect Camera::GetCameraActiveRange() const {
    return m_camera_active_range;
}

float Camera::SetBorder(const float newborder) {
    m_border = newborder;
    return newborder;
}

float Camera::GetBorder() const {
    return m_border;
}

bool Camera::CanCameraActive() const {
    if (m_world_pos.x < m_camera_active_range.x ||
        m_world_pos.y < m_camera_active_range.y ||
        m_world_pos.x > m_camera_active_range.w + m_camera_active_range.x ||
        m_world_pos.y > m_camera_active_range.h + m_camera_active_range.y)
        return false;
    return true;
}
