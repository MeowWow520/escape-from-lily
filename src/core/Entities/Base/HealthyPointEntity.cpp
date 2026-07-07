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
