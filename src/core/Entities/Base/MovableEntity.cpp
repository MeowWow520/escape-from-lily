//
// Created by MeowWow520 on 2026/5/14.
//

#include "MovableEntity.h"



int MovableEntity::Initialize() {
    m_max_speed = m_config_manager.GetPlayerJson().max_speed;
    if (m_config_manager.GetDefaultJson().feature.acceleration)
        // TODO: acceleration
        m_acceleration = 0;
    return 0;
}

float MovableEntity::GetMaxSpeed() const {
    return m_max_speed;
}

float MovableEntity::SetMaxSpeed(const float newmaxspeed) {
    m_max_speed = newmaxspeed;
    return newmaxspeed;
}

float MovableEntity::GetAcceleration() const {
    return m_acceleration;
}

float MovableEntity::SetAcceleration(const float newacceleration) {
    m_acceleration = newacceleration;
    return newacceleration;
}

glm::vec2 MovableEntity::GetVector() const {
    return m_vector;
}

glm::vec2 MovableEntity::SetVector(const glm::vec2 newvector) {
    m_vector = newvector;
    return newvector;
}
