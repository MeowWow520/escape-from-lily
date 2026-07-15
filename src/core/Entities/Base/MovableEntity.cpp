//
// Created by MeowWow520 on 2026/5/14.
//

#include "MovableEntity.h"



int MovableEntity::Initialize() {
    m_fMaxSpeed = m_config_manager.getDefaultJson().player_params.max_speed;
    if (m_config_manager.getDefaultJson().feature.acceleration)
        // TODO: acceleration
        m_fAcceleration = 0;
    return 0;
}

float MovableEntity::getMaxSpeed() const {
    return m_fMaxSpeed;
}

float MovableEntity::setMaxSpeed(const float new_max_speed) {
    m_fMaxSpeed = new_max_speed;
    return new_max_speed;
}

float MovableEntity::getAcceleration() const {
    return m_fAcceleration;
}

float MovableEntity::setAcceleration(const float new_acceleration) {
    m_fAcceleration = new_acceleration;
    return new_acceleration;
}

glm::vec2 MovableEntity::getVector() const {
    return m_vec2_vector;
}

glm::vec2 MovableEntity::setVector(const glm::vec2 new_vector) {
    m_vec2_vector = new_vector;
    return new_vector;
}
