//
// Created by MeowWow520 on 2026/5/14.
//

#include "KeyboardInput.h"

#include <ranges>

#include "../Def.h"
#include "../Logger/Log.h"


KeyboardInput::KeyboardInput() = default;


void KeyboardInput::handleEvents(const SDL_Event event) {
    // 如果不是键盘事件，跳过
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP)
        return;
    const auto it = m_map_keyBind.find(event.key.key);
    // 如果未绑定事件，跳过
    if (it == m_map_keyBind.end()) return;
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (m_map_currentActionState[it->second] == ActionState::Idle) {
            if (m_config_manager.getDefaultJson().feature.key_logging)
                EFL_LOGGER_INFO(LogCategory::Input, "SDLK_{} down", SDL_GetKeyName(event.key.key));
            m_map_currentActionState[it->second] = ActionState::Pressed;
        }
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        if (m_map_currentActionState[it->second] == ActionState::Held) {
            if (m_config_manager.getDefaultJson().feature.key_logging)
                EFL_LOGGER_INFO(LogCategory::Input, "SDLK_{} up", SDL_GetKeyName(event.key.key));
            m_map_currentActionState[it->second] = ActionState::Released;
        }
    }
}

void KeyboardInput::Update(const float dt) {
    for (auto &action: m_map_keyBind | std::views::values) {
        if (m_map_currentActionState[action] == ActionState::Pressed)
            m_map_currentActionState[action]  = ActionState::Held;
        if (m_map_currentActionState[action] == ActionState::Released)
            m_map_currentActionState[action]  = ActionState::Idle;
    }
}

glm::vec2 KeyboardInput::GetMovementNormalizeVec2() {
    glm::vec2 ret(0.0f);
    if (m_map_currentActionState[Action::MoveUp]    == ActionState::Held) ret.y -= 1.0f; // SDLK_W
    if (m_map_currentActionState[Action::MoveDown]  == ActionState::Held) ret.y += 1.0f; // SDLK_S
    if (m_map_currentActionState[Action::MoveLeft]  == ActionState::Held) ret.x -= 1.0f; // SDLK_A
    if (m_map_currentActionState[Action::MoveRight] == ActionState::Held) ret.x += 1.0f; // SDLK_D
    if (ret.x != 0.0f && ret.y != 0.0f)
        ret = glm::normalize(ret);
    return ret;
}

int KeyboardInput::setDefaultKeyBind() {
    EFL_CHECK(LogCategory::Input, bindAction(SDLK_W, Action::MoveUp),    "Bind SDLK_W to Action::MoveUp");
    EFL_CHECK(LogCategory::Input, bindAction(SDLK_S, Action::MoveDown),  "Bind SDLK_S to Action::MoveDown");
    EFL_CHECK(LogCategory::Input, bindAction(SDLK_D, Action::MoveRight), "Bind SDLK_D to Action::MoveRight");
    EFL_CHECK(LogCategory::Input, bindAction(SDLK_A, Action::MoveLeft),  "Bind SDLK_A to Action::MoveLeft");
    EFL_CHECK(LogCategory::Input, bindAction(SDLK_ESCAPE, Action::Quit), "Bind SDLK_ESCAPE to Action::Quit");
    // 初始化两个哈希表
    for (auto it = m_map_keyBind.begin(); it != m_map_keyBind.end(); ++it)
        m_map_currentActionState[it->second]  = ActionState::Idle;
    return 0;
}