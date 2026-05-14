//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

int Background::Initialize() {

    if (!InitializeTextureFromPath()) {
        m_return_code = -1;
        goto to_quit;
    }
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}

void Background::HandleEvents(const SDL_Event event) {
    (void)event;
}

void Background::Update(float dt) {

}

void Background::Render() {
    // FIXME: 修复此处的 bug
    SDL_RenderTexture(m_game_instance.GetSDLRenderer(), m_texture.get(), nullptr, nullptr);
}

int Background::Quit() {
    goto to_quit;
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassQuit(m_return_code, loc);
}
