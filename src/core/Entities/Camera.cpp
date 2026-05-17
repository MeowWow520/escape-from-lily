//
// Created by MeowWow520 on 2026/5/17.
//

#include "Camera.h"
#include "../Scene.h"


int Camera::Initialize() {
    m_border = 100.0f;
    m_camera_active_range= {
        m_border, m_border,
        m_game_instance.GetCurrentScene()->GetWorldSize().x - 2 * m_border,
        m_game_instance.GetCurrentScene()->GetWorldSize().y - 2 * m_border,
    };
    m_visible = false;
    const SDL_FRect window = {0, 0,
        m_game_instance.GetWindowSize().x,
        m_game_instance.GetWindowSize().y
    };
    SDL_FRect hitbox_size = {0, 0, 0, 0};
    if (!SDL_GetRectIntersectionFloat(&window,&m_camera_active_range, &hitbox_size)) {
        m_return_code = -1;
        goto to_quit;
    }
    m_entity_hitbox = {hitbox_size.w, hitbox_size.h};
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}

void Camera::Update(const float dt) {
    // if (CanCameraActive())
    //       m_game_instance.GetCurrentScene()->GetCameraPos() += m_game_instance.GetKeyboardInput()->GetMovementNormalizeVec2() * m_max_speed * dt;
}

SDL_FRect Camera::SetCameraActiveRange(const SDL_FRect newactiverange) {
    m_camera_active_range = newactiverange;
    return newactiverange;
}

SDL_FRect Camera::GetCameraActiveRange() const {
    return m_camera_active_range;
}

float Camera::SetBorder(float newborder) {
    m_border = newborder;
    return newborder;
}

float Camera::GetBorder() const {
    return m_border;
}

bool Camera::CanCameraActive() const {
    if (m_world_pos.x < m_border ||
        m_world_pos.y < m_border ||
        m_world_pos.x > m_game_instance.GetCurrentScene()->GetWorldSize().x - m_game_instance.GetWindowSize().x ||
        m_world_pos.y > m_game_instance.GetCurrentScene()->GetWorldSize().y - m_game_instance.GetWindowSize().y)
        return false;
    return true;
}
