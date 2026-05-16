//
// Created by MeowWow520 on 2026/5/14.
//

#include "Input.h"


Input::Input() {
    m_return_code = 0;

}

void Input::Update(float dt) {
}

ActionState Input::GetActionState(const Action action) const {
    return m_current_action_state.find(action)->second;
}

bool Input::IsActionPressed(const Action action) const {
    return (m_current_action_state.find(action)->second == ActionState::Pressed);
}

bool Input::IsActionHeld(const Action action) const {
    return (m_current_action_state.find(action)->second == ActionState::Held);
}

bool Input::IsActionReleased(const Action action) const {
    return (m_current_action_state.find(action)->second == ActionState::Released);
}

bool Input::BindAction(const SDL_Keycode key, Action action) {
    m_key_bind.try_emplace(key, action);
    return true;
}

bool Input::UnbindAction(const Action action) {
    for (auto it = m_key_bind.begin(); it != m_key_bind.end(); ++it) {
        if (it->second == action) {
            m_key_bind.erase(it);
            return true;
        }
    }
    return false;
}

bool Input::SetDefaultKeyBind() {
    return true;
}

void Input::SetActionState(const Action action, const ActionState state) {
    // TODO: 检测
    m_current_action_state[action] = state;
}
