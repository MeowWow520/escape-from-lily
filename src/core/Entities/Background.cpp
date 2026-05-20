//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

#include "../Scene.h"

int Background::Initialize() {
    if (!InitializeTextureFromPath()) return -1;
    m_rect = {0, 0, 0, 0};
    m_world_pos = glm::vec2(0.0f, 0.0f);
    m_screen_pos = TransScreenPos();
    return 0;
}


void Background::Update(float dt) {
    m_screen_pos = TransScreenPos();
}

void Background::Render() {
    const SDL_FRect destination = EFL_Vec2AddToRectFloat(m_screen_pos, m_game_instance.GetCurrentScene()->GetWorldSize());
    SDL_RenderTexture(m_game_instance.GetSDLRenderer(), m_texture, nullptr, &destination);
}

int Background::Quit() {
    if (m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    return 0;
}
