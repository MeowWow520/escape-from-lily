//
// Created by MeowWow520 on 2026/5/17.
//

#include "Player.h"

int Player::Initialize() {
    return MovableEntity::Initialize();
}

void Player::HandleEvents(SDL_Event event) {
    MovableEntity::HandleEvents(event);
}

void Player::Update(float dt) {
    MovableEntity::Update(dt);
}

void Player::Render() {
    MovableEntity::Render();
}

int Player::Quit() {
    return MovableEntity::Quit();
}

std::string Player::SetName(std::string name) {
    m_player_name = name;
    return name;
}
