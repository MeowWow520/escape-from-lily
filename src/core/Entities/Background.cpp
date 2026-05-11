//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

int Background::Initialize() {
    if (!InitializeTextureFromPath(m_path.c_str()))
        return -1;

    return TexturedEntity::Initialize();
}

void Background::HandleEvents(SDL_Event event) {

}

void Background::Update(float dt) {

}

void Background::Render() {

}

int Background::Quit() {
    return TexturedEntity::Quit();
}
