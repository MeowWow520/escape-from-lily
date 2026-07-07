//
// Created by MeowWow520 on 2026/5/8.
//

#include "Game.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Def.h"
#include "Scene.h"
#include "../SceneMain.h"
#include "Config/ConfigManager.h"
#include "Font/FontManager.h"
#include "Logger/Log.h"


Game::Game() :
    font_manager(FontManager::getInstance()),
    config_manager(ConfigManager::getInstance())
{ }

int Game::Initialize() {

    m_strTitle = config_manager.getDefaultJson().display.window.title;
    m_vec2_windowSize = config_manager.getDefaultJson().display.window.size;
    m_bRunning = true;
    m_fDeltaTime = 0.0f;
    m_i32FPS = config_manager.getDefaultJson().display.fps;
    m_i32FrameDelay = static_cast<Uint32>(1e9) / m_i32FPS;
    m_window = nullptr;
    m_renderer = nullptr;
    m_currentScene = nullptr;


    // 初始化字体管理
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Core, SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), "SDL_Init");
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Core, TTF_Init(), "TTF_Init");
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Core, MIX_Init(), "MIX_Init");
    const bool SDL_CreateWR = SDL_CreateWindowAndRenderer(m_strTitle.c_str(),
        static_cast<int>(m_vec2_windowSize.x),
        static_cast<int>(m_vec2_windowSize.y),
        SDL_WINDOW_RESIZABLE,&m_window, &m_renderer);
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Core, SDL_CreateWR, "SDL_CreateWindowAndRenderer");
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Core, SDL_SetRenderVSync(m_renderer, 1), "SDL_SetRenderVSync");
    if (font_manager.Initialize(*this) != 0) return -1;

    // 设置按键绑定
    m_keyInput = new KeyboardInput();
    EFL_CHECK(LogCategory::Input, m_keyInput->setDefaultKeyBind() == 0, "Keyboard SetDefaultKeyBind");

    // 创建场景
    m_currentScene = new SceneMain();
    EFL_CHECK(LogCategory::Input, m_currentScene->Initialize() == 0, "scene Initialize");

    return 0;
}

int Game::Running() {

    while (m_bRunning) {
        const Uint64 start = SDL_GetTicksNS();

        handleEvents();
        Update(m_fDeltaTime);
        Render();

        const Uint64 end = SDL_GetTicksNS();
        if (const Uint64 elapsed = end - start; elapsed < m_i32FrameDelay) {
            SDL_DelayNS(m_i32FrameDelay - elapsed);
            m_fDeltaTime = static_cast<float>(m_i32FrameDelay / 1e9);
        } else {
            m_fDeltaTime = static_cast<float>(static_cast<double>(elapsed) / 1e9);
        }
    }
    Quit();
    EFL_CHECK(LogCategory::Core, config_manager.Quit() == 0, "Config manager quit");
    return 0;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_keyInput->handleEvents(event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                EFL_LOGGER_INFO(LogCategory::Scene, "Receiving SDL_EVENT_QUIT, main loop quitting");
                m_bRunning = false;
                break;
            default:
                m_currentScene->handleEvents(event);
                break;
        }
    }
}

void Game::Update(const float dt) const {
    m_currentScene->Update(dt);
    m_keyInput->Update(dt);
}

void Game::Render() const {
    SDL_RenderClear(m_renderer);
    m_currentScene->Render();
    SDL_RenderPresent(m_renderer);
}


int Game::Quit() {
    if (m_currentScene != nullptr) {
        if (m_currentScene->Quit() != 0) {
            EFL_LOGGER_ERROR(LogCategory::Core, "scene Quit failed");
        }
    }
    delete m_currentScene;
    m_currentScene = nullptr;
    delete m_keyInput;
    m_keyInput = nullptr;
    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    TTF_Quit();
    EFL_CHECK(LogCategory::Core, font_manager.Quit() == 0, "font_manager quit");
    MIX_Quit();
    SDL_Quit();
    return 0;
}

glm::vec2 Game::getWindowSize() const {
    return m_vec2_windowSize;
}

bool Game::setRunning(const bool new_running) {
    m_bRunning = new_running;
    return new_running;
}

bool Game::getRunning() const {
    return m_bRunning;
}

Uint32 Game::getFPS() const {
    return m_i32FPS;
}

SDL_Window* Game::getSDLWindow() const {
    return m_window;
}

SDL_Renderer* Game::getSDLRenderer() const {
    return m_renderer;
}

Scene * Game::getCurrentScene() const {
    return m_currentScene;
}

KeyboardInput* Game::getKeyboardInput() const {
    return m_keyInput;
}
