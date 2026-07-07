//
// Created by MeowWow520 on 2026/5/23.
//

#include "FontManager.h"

#include "../Game.h"
#include "../Logger/Log.h"



int FontManager::Initialize(Game& game) {
    m_game_instance = &game;
    m_text_engine = TTF_CreateRendererTextEngine(m_game_instance->getSDLRenderer());
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Font, m_text_engine, "TTF_CreateRendererTextEngine failed");
    return 0;
}

void FontManager::handleEvents(SDL_Event event) {
}

void FontManager::Update(float dt) {
}

void FontManager::Render() {
}

int FontManager::Quit() {
    if (m_text_engine != nullptr) {
        TTF_DestroyRendererTextEngine(m_text_engine);
        m_text_engine = nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Font, "TTF_DestroyRendererTextEngine successful");
    return 0;
}

TTF_TextEngine* FontManager::getTTFEngine() const {
    return m_text_engine;
}


