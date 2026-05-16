//
// Created by MeowWow520 on 2026/5/14.
//

#include "KeyboardInput.h"

#include "../Def.h"


KeyboardInput::KeyboardInput() {
    m_return_code = 0;
    KeyboardInput::SetDefaultKeyBind();
    // 初始化两个哈希表
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it)
        m_preframe_action_state[it->second] = ActionState::Idle;
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it)
        m_current_action_state[it->second]  = ActionState::Idle;
}

void KeyboardInput::HandleEvents(SDL_Event event) {
}

void KeyboardInput::Update(const float dt) {// 遍历
    for (auto& [action, state] : m_key_bind) {
        // 复制本帧到上一帧
        if (m_current_action_state[state] == ActionState::Pressed)
            m_current_action_state[state]  = ActionState::Held;
        if (m_current_action_state[state] == ActionState::Released)
            m_current_action_state[state]  = ActionState::Idle;
        m_preframe_action_state[state] = m_current_action_state[state]; // 本操作应该在哪里?
    }
    // FIXME: ？烧脑的逻辑!
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

to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}