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
#include "Font/FontManager.h"
#include "Logger/Log.h"


Game::Game()
    : font_manager(FontManager::GetInstance())
{
    m_title = "escape-from-lily";
    m_window_size = {1920, 1080};
    m_running = true;
    m_delta_time = 0.0f;
    m_FPS = 120;
    m_frame_delay = static_cast<Uint32>(1e9) / m_FPS;
    m_window = nullptr;
    m_renderer = nullptr;
    m_current_scene = nullptr;
    m_return_code = 0;
}

int Game::Initialize() {
    // 初始化字体管理
    EFL_CHACK_WITH_GET_ERROR(LogCategory::Core, SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), "SDL_Init");
    EFL_CHACK_WITH_GET_ERROR(LogCategory::Core, TTF_Init(), "TTF_Init");
    EFL_CHACK_WITH_GET_ERROR(LogCategory::Core, MIX_Init(), "MIX_Init");
    const bool SDL_CreateWR = SDL_CreateWindowAndRenderer(m_title.c_str(),
        static_cast<int>(m_window_size.x),
        static_cast<int>(m_window_size.y),
        SDL_WINDOW_RESIZABLE,&m_window, &m_renderer);
    EFL_CHACK_WITH_GET_ERROR(LogCategory::Core, SDL_CreateWR, "SDL_CreateWindowAndRenderer");
    EFL_CHACK_WITH_GET_ERROR(LogCategory::Core, SDL_SetRenderVSync(m_renderer, 1), "SDL_SetRenderVSync");
    if (font_manager.Initialize(*this) != 0) return -1;

    // 设置按键绑定
    m_key_input = new KeyboardInput();
    EFL_CHECK(LogCategory::Input, m_key_input->SetDefaultKeyBind(), "Keyboard SetDefaultKeyBind");

    // TODO: 使用工厂方法注册对象
    // 创建场景
    m_current_scene = new SceneMain();
    EFL_CHECK(LogCategory::Input, m_current_scene->Initialize() == 0, "scene Initialize");

    return 0;
}

int Game::Running() {

    while (m_running) {
        const Uint64 start = SDL_GetTicksNS();

        HandleEvents();
        Update(m_delta_time);
        Render();

        const Uint64 end = SDL_GetTicksNS();
        if (const Uint64 elapsed = end - start; elapsed < m_frame_delay) {
            SDL_DelayNS(m_frame_delay - elapsed);
            m_delta_time = static_cast<float>(m_frame_delay / 1e9);
        } else {
            m_delta_time = static_cast<float>(static_cast<double>(elapsed) / 1e9);
        }
    }
    Quit();
    return 0;
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_key_input->HandleEvents(event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                EFL_LOGGER_INFO(LogCategory::Scene, "Receiving SDL_EVENT_QUIT, main loop quitting");
                m_running = false;
                break;
            default:
                m_current_scene->HandleEvents(event);
                break;
        }
    }
}

void Game::Update(const float dt) const {
    m_current_scene->Update(dt);
    m_key_input->Update(dt);
}

void Game::Render() const {
    SDL_SetRenderDrawColor(m_renderer, COLOR(HEX_COLOR_BACKGROUND));
    SDL_RenderClear(m_renderer);
    m_current_scene->Render();
    SDL_RenderPresent(m_renderer);
}


int Game::Quit() {
    if (m_current_scene != nullptr) {
        if (m_current_scene->Quit() != 0) {
            EFL_LOGGER_ERROR(LogCategory::Core, "scene Quit failed");
        }
    }
    delete m_current_scene;
    m_current_scene = nullptr;
    delete m_key_input;
    m_key_input = nullptr;
    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    TTF_Quit();
    EFL_CHECK(LogCategory::Core, font_manager.Quit(), "font_manager quit");
    MIX_Quit();
    SDL_Quit();
    return 0;
}

glm::vec2 Game::GetWindowSize() const {
    return m_window_size;
}

bool Game::SetRunning(const bool newrunning) {
    m_running = newrunning;
    return newrunning;
}

bool Game::GetRunning() const {
    return m_running;
}

Uint32 Game::GetFPS() const {
    return m_FPS;
}

SDL_Window * Game::GetSDLWindow() const {
    return m_window;
}

SDL_Renderer * Game::GetSDLRenderer() const {
    return m_renderer;
}

Scene * Game::GetCurrentScene() const {
    return m_current_scene;
}

KeyboardInput * Game::GetKeyboardInput() const {
    return m_key_input;
}
