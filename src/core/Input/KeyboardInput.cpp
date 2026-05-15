//
// Created by MeowWow520 on 2026/5/14.
//

#include "KeyboardInput.h"

#include "../Def.h"


KeyboardInput::KeyboardInput() {
    m_return_code = 0;
    KeyboardInput::SetDefaultKeyBind();
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it)
        m_preframe_action_state[it->second] = ActionState::Idle;
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it)
        m_current_action_state[it->second]  = ActionState::Idle;
}

void KeyboardInput::HandleEvents(SDL_Event event) {
}

void KeyboardInput::Update(const float dt) {
    // FIXME: 神奇的逻辑？烧脑的逻辑!
    for (auto it = m_current_action_state.begin(); it != m_current_action_state.end(); ++it) {
        m_preframe_action_state[it->first] = m_current_action_state[it->first];
        if (it->second == ActionState::Pressed) {
            m_current_action_state[it->first] = ActionState::Held;
        } else if (it->second == ActionState::Released) {
            m_current_action_state[it->first] = ActionState::Idle;
        } else {
            m_current_action_state[it->first] = ActionState::Idle;
        }
    }
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

to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}