//
// Created by MeowWow520 on 2026/5/14.
//

#include "MovableEntity.h"


MovableEntity::MovableEntity() {
    m_max_speed = DEFAULT_MAX_SPEED;
    if (SWITCHER_ACCELERATION)
        m_acceleration = DEFAULT_ACCELERATION;
}

float MovableEntity::GetMaxSpeed() const {
    return m_max_speed;
}

float MovableEntity::SetMaxSpeed(const float newmaxspeed) {
    m_max_speed = newmaxspeed;
    return newmaxspeed;
}
