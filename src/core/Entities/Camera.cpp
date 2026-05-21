//
// Created by MeowWow520 on 2026/5/17.
//

#include "Camera.h"
#include "../Scene.h"
#include <algorithm>


int Camera::Initialize() {
    m_visible = false;
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
    // 计算理想相机位置：玩家中心对齐屏幕中心
    const Player* player = m_game_instance.GetCurrentScene()->GetPlayer();
    glm::vec2 target = {
        player->GetWorldPos().x + player->GetTextureSize().x / 2.0f - m_game_instance.GetWindowSize().x / 2.0f,
        player->GetWorldPos().y + player->GetTextureSize().y / 2.0f - m_game_instance.GetWindowSize().y / 2.0f
    };
    // 限制相机在合法范围
    target.x = std::clamp(target.x,
        m_camera_active_range.x,
        m_camera_active_range.x + m_camera_active_range.w);
    target.y = std::clamp(target.y,
        m_camera_active_range.y,
        m_camera_active_range.y + m_camera_active_range.h);
    // 更新相机位置
    m_world_pos = target;
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
