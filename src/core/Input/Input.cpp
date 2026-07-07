//
// Created by MeowWow520 on 2026/5/14.
//

#include "Input.h"


Input::Input() {
}

void Input::Update(float dt) {
}

ActionState Input::getActionState(const Action action) const {
    return m_map_currentActionState.find(action)->second;
}

bool Input::isActionPressed(const Action action) const {
    return (m_map_currentActionState.find(action)->second == ActionState::Pressed);
}

bool Input::isActionHeld(const Action action) const {
    return (m_map_currentActionState.find(action)->second == ActionState::Held);
}

bool Input::isActionReleased(const Action action) const {
    return (m_map_currentActionState.find(action)->second == ActionState::Released);
}

bool Input::bindAction(const SDL_Keycode key, Action action) {
    m_map_keyBind.try_emplace(key, action);
    return true;
}

bool Input::unbindAction(const Action action) {
    for (auto it = m_map_keyBind.begin(); it != m_map_keyBind.end(); ++it) {
        if (it->second == action) {
            m_map_keyBind.erase(it);
            return true;
        }
    }
    return false;
}

int Input::setDefaultKeyBind() {
    return 0;
}

void Input::setActionState(const Action action, const ActionState state) {
    // TODO: 检测
    m_map_currentActionState[action] = state;
}
