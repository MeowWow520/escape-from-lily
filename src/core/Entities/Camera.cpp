//
// Created by MeowWow520 on 2026/5/17.
//

#include "Camera.h"
#include "../Scene.h"
#include <algorithm>


int Camera::Initialize() {
    m_bVisible = false;
    m_cameraActiveRange= { 0, 0,
        m_game_instance.getCurrentScene()->GetWorldSize().x - m_game_instance.getWindowSize().x,
        m_game_instance.getCurrentScene()->GetWorldSize().y - m_game_instance.getWindowSize().y
    };

    // 设置碰撞箱
    const SDL_FRect window = {0, 0,
        m_game_instance.getWindowSize().x,
        m_game_instance.getWindowSize().y
    };
    // FIXME: 无用的变量？
    SDL_FRect hitbox_size = {0, 0, 0, 0};
    SDL_GetRectIntersectionFloat(&window,&m_cameraActiveRange, &hitbox_size);
    m_vec2_entityHitbox = {hitbox_size.w, hitbox_size.h};

    return 0;
}

void Camera::Update(const float dt) {
    // 计算理想相机位置：玩家中心对齐屏幕中心
    const Player* player = m_game_instance.getCurrentScene()->GetPlayer();
    glm::vec2 target = {
        player->getWorldPos().x + player->getTextureSize().x / 2.0f - m_game_instance.getWindowSize().x / 2.0f,
        player->getWorldPos().y + player->getTextureSize().y / 2.0f - m_game_instance.getWindowSize().y / 2.0f
    };
    // 限制相机在合法范围
    target.x = std::clamp(target.x,
        m_cameraActiveRange.x,
        m_cameraActiveRange.x + m_cameraActiveRange.w);
    target.y = std::clamp(target.y,
        m_cameraActiveRange.y,
        m_cameraActiveRange.y + m_cameraActiveRange.h);
    // 更新相机位置
    m_vec2_worldPos = target;
}


SDL_FRect Camera::getCameraActiveRange() const {
    return m_cameraActiveRange;
}

float Camera::setBorder(const float new_border) {
    m_fBorder = new_border;
    return new_border;
}

float Camera::getBorder() const {
    return m_fBorder;
}
