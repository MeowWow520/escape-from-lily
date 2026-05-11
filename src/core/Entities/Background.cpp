//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

int Background::Initialize() {
    // 设置纹理
    m_path = "assets/images/test.jpg";
    m_texture.reset(nullptr);
    if (SetTextureFromPath(m_path.c_str()))
        return -1;
    bool m_visible = true;
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
