//
// Created by MeowWow520 on 2026/5/11.
//

#include "Background.h"

#include <SDL3_image/SDL_image.h>

#include "../Scene.h"
#include "../Logger/Log.h"

int Background::Initialize() {
    m_bVisible = true;
    if (!m_strPath.data() || m_strPath[0] == '\0') {
        EFL_LOGGER_ERROR(LogCategory::Entity, "Background Initialize() failed: m_path is NULL");
        return -1;
    }
    m_texture = IMG_LoadTexture(m_game_instance.getSDLRenderer(), m_strPath.c_str());
    EFL_CHECK(LogCategory::Entity, m_texture != nullptr, "Background IMG_LoadTexture");
    SDL_GetTextureSize(m_texture,&m_vec2_textureSize.x,&m_vec2_textureSize.y);
    m_vec2_worldPos = {0, 0};
    m_vec2_screenPos = transScreenPos();
    return 0;
}


void Background::Update(float dt) {
    m_vec2_screenPos = transScreenPos();
}

void Background::Render() {
    const SDL_FRect destination = EFL_Vec2AddToRectFloat(m_vec2_screenPos, m_game_instance.getCurrentScene()->GetWorldSize());
    SDL_RenderTexture(m_game_instance.getSDLRenderer(), m_texture, nullptr, &destination);
}

int Background::Quit() {
    if (m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    return 0;
}
