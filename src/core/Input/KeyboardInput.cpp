//
// Created by MeowWow520 on 2026/5/14.
//

#include "KeyboardInput.h"

#include <ranges>

#include "../Def.h"


KeyboardInput::KeyboardInput() {
    m_return_code = 0;
}

void KeyboardInput::HandleEvents(SDL_Event event) {
    // 如果不是键盘事件，跳过
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP)
        return;
    const auto it = m_key_bind.find(event.key.key);
    // 如果未绑定事件，跳过
    if (it == m_key_bind.end()) return;
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (m_current_action_state[it->second] == ActionState::Idle) {
            if (SWITCHER_KEYLOGGING)
                spdlog::info("SDL Key_{} Down", SDL_GetKeyName(event.key.key));
            m_current_action_state[it->second] = ActionState::Pressed;
        }
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        if (m_current_action_state[it->second] == ActionState::Held) {
            if (SWITCHER_KEYLOGGING)
                spdlog::info("SDL Key_{} Up", SDL_GetKeyName(event.key.key));
            m_current_action_state[it->second] = ActionState::Released;
        }
    }
}

void KeyboardInput::Update(const float dt) {
    for (auto &action: m_key_bind | std::views::values) {
        if (m_current_action_state[action] == ActionState::Pressed)
            m_current_action_state[action]  = ActionState::Held;
        if (m_current_action_state[action] == ActionState::Released)
            m_current_action_state[action]  = ActionState::Idle;
    }
}

glm::vec2 KeyboardInput::GetMovementNormalizeVec2() {
    glm::vec2 ret(0.0f);
    if (m_current_action_state[Action::MoveUp]    == ActionState::Held) ret.y += 1.0f; // SDLK_W
    if (m_current_action_state[Action::MoveDown]  == ActionState::Held) ret.y -= 1.0f; // SDLK_S
    if (m_current_action_state[Action::MoveLeft]  == ActionState::Held) ret.x -= 1.0f; // SDLK_A
    if (m_current_action_state[Action::MoveRight] == ActionState::Held) ret.x += 1.0f; // SDLK_D
    if (ret.x != 0.0f && ret.y != 0.0f)
        ret = glm::normalize(ret);
    return ret;
}

bool KeyboardInput::SetDefaultKeyBind() {
    if (!BindAction(SDLK_W, Action::MoveUp)) {
        m_return_code = -1;
        goto to_quit;
    }
    if (!BindAction(SDLK_S, Action::MoveDown)) {
        m_return_code = -1;
        goto to_quit;
    }
    if (!BindAction(SDLK_D, Action::MoveRight)) {
        m_return_code = -1;
        goto to_quit;
    }
    if (!BindAction(SDLK_A, Action::MoveLeft)) {
        m_return_code = -1;
        goto to_quit;
    }
    if (!BindAction(SDLK_ESCAPE, Action::Quit)) {
        m_return_code = -1;
        goto to_quit;
    }
    // 初始化两个哈希表
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it)
        m_current_action_state[it->second]  = ActionState::Idle;
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}
