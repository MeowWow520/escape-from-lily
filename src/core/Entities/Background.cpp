//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

#include <SDL3_image/SDL_image.h>

#include "../Scene.h"
#include "../Logger/Log.h"

int Background::Initialize() {
    m_visible = true;
    if (!m_path.data() || m_path[0] == '\0') {
        EFL_LOGGER_ERROR(LogCategory::Entity, "Background Initialize() failed: m_path is NULL");
        return -1;
    }
    m_texture = IMG_LoadTexture(m_game_instance.GetSDLRenderer(), m_path.c_str());
    EFL_CHECK(LogCategory::Entity, m_texture != nullptr, "Background IMG_LoadTexture");
    SDL_GetTextureSize(m_texture,&m_texture_size.x,&m_texture_size.y);
    m_world_pos = {0, 0};
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
