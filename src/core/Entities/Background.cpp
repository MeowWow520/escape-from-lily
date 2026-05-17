//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

#include "../Scene.h"

int Background::Initialize() {
    if (!InitializeTextureFromPath()) {
        m_return_code = -1;
        goto to_quit;
    }
    m_rect = {0, 0, 0, 0};
    m_world_pos = glm::vec2(0.0f, 0.0f);
    m_screen_pos = TransScreenPos();
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}

void Background::HandleEvents(const SDL_Event event) {
    (void)event;
}

void Background::Update(float dt) {
    m_screen_pos = TransScreenPos();
}

void Background::Render() {
    // FIXME: 修复此处的 bug
    const SDL_FRect destination = EFL_Vec2AddToRectFloat(m_screen_pos, m_game_instance.GetCurrentScene()->GetWorldSize());
    SDL_RenderTexture(m_game_instance.GetSDLRenderer(), m_texture.get(), nullptr, &destination);
}

int Background::Quit() {
    // TODO: 释放资源
    goto to_quit;
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassQuit(m_return_code, loc);
}
