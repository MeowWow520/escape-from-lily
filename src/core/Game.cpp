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
#include "Logger/Log.h"


Game::Game() {
    m_title = "escape-from-lily";
    m_window_size = {1920, 1080};
    m_running = true;
    m_delta_time = 0.0f;
    m_frame_delay = 0.0f;
    m_FPS = 120;
    m_window = nullptr;
    m_renderer = nullptr;
    m_current_scene = nullptr;
    m_return_code = 0;
}

int Game::Initialize() {
    EFL::RegisterLogCategory();

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) return -1;
    if (!TTF_Init()) return -1;
    if (!MIX_Init()) return -1;
    if (!SDL_CreateWindowAndRenderer(m_title.c_str(),
            static_cast<int>(m_window_size.x),
            static_cast<int>(m_window_size.y),
            SDL_WINDOW_RESIZABLE,&m_window, &m_renderer)) { return -1; }
    // 设置渲染器 -- 垂直同步
    if (!SDL_SetRenderVSync(m_renderer, 1)) return -1;

    // 设置按键绑定
    m_key_input = new KeyboardInput();
    if (!m_key_input->SetDefaultKeyBind()) return -1;

    // TODO: 使用工厂方法注册对象
    // 创建场景
    m_current_scene = new SceneMain();
    if (m_current_scene->Initialize() != 0) {
        delete m_current_scene;
        return -1;
    }

    return 0;
}

int Game::Running() {

    while (m_running) {
        const Uint64 start = SDL_GetTicksNS();

        HandleEvents();
        Update(m_delta_time);
        Render();

        const Uint64 end = SDL_GetTicksNS();
        const Uint64 elapsed = end - start;
        if (elapsed < m_frame_delay) {
            SDL_DelayNS(m_frame_delay - elapsed);
            m_delta_time = static_cast<float>(m_frame_delay / 1e9);
        } m_delta_time = static_cast<float>(static_cast<double>(elapsed) / 1e9);
    }
    return 0;
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_key_input->HandleEvents(event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
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
    // 释放游戏资源
    if (m_current_scene != nullptr) {
        if (m_current_scene->Quit() != 0) return -1;
    } delete m_current_scene;
    // 释放 SDL 资源
    TTF_Quit();
    MIX_Quit();
    SDL_Quit();
    EFL::QuitLogger();
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
