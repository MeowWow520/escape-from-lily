//
// Created by MeowWow520 on 2026/5/23.
//

#include "FontManager.h"

#include "../Logger/Log.h"



int FontManager::Initialize() {
    TTF_TextEngine* engine = TTF_CreateRendererTextEngine(m_game_instance.GetSDLRenderer());
    if (!engine) {
        EFL_LOGGER_ERROR(LogCategory::Core,
            "TTF_CreateRendererTextEngine failed : {}", SDL_GetError());
        return -1;
    }
    return 0;
}

void FontManager::HandleEvents(SDL_Event event) {
}

void FontManager::Update(float dt) {
}

void FontManager::Render() {
}

int FontManager::Quit() {
    TTF_DestroyRendererTextEngine(m_text_engine);
    return 0;
}

TTF_TextEngine* FontManager::GetTTFEngine() const {
    return m_text_engine;
}


