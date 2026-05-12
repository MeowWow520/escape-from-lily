//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

int Background::Initialize() {

    if (!InitializeTextureFromPath())
        return -1;

    return TexturedEntity::Initialize();
}

void Background::HandleEvents(SDL_Event event) {

}

void Background::Update(float dt) {

}

void Background::Render() {
    SDL_RenderTexture(m_game_instance.GetSDLRenderer(), m_texture.get(), nullptr, nullptr);
}

int Background::Quit() {
    return TexturedEntity::Quit();
}
