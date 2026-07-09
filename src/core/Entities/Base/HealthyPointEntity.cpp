//
// Created by MeowWow520 on 2026/5/28.
//

#include "HealthyPointEntity.h"

int HealthyPointEntity::Initialize() {
    return MovableEntity::Initialize();
}

void HealthyPointEntity::handleEvents(SDL_Event event) {
    MovableEntity::handleEvents(event);
}

void HealthyPointEntity::Update(float deltaTime) {
    MovableEntity::Update(deltaTime);
}

void HealthyPointEntity::Render() {
    MovableEntity::Render();
}

int HealthyPointEntity::Quit() {
    return MovableEntity::Quit();
}

float HealthyPointEntity::getHealth() const {
    return m_fHealth;
}

float HealthyPointEntity::setHealth(float new_health) {
    m_fHealth = new_health;
    return new_health;
}

float HealthyPointEntity::getGetDamage() const {
    return m_fGetDamage;
}
float HealthyPointEntity::setGetDamage(float new_damage) {
    m_fGetDamage = new_damage;
    return new_damage;
}
