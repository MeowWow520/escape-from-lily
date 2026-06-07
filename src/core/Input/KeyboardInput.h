//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#define ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#include "Input.h"
#include <glm/glm.hpp>
#include "../Config/ConfigManager.h"



class KeyboardInput : public Input {
    public:
        KeyboardInput();

        // 设置键盘的默认按键绑定
        int SetDefaultKeyBind() override;
        /**
         * HandleEvents 通过处理 SDL_Event 来获取键盘上哪些按键被按下，哪些松开
         *
         * @param event 要处理的 SDL 事件系统
         */
        void HandleEvents(SDL_Event event) override;

        /**
         * Update 更新 m_current_action_state 表格：
         * Idle --> Idle || Pressed --> Held || Held --> Held || Released --> Idle
         *
         * @param dt delta time
         */
        void Update(float dt) override;

        /**
         * GetMovementNormalizeVec2 通过 m_current_action_state 中移动方向的按键来计算得到实体移动方向的单位向量
         *
         * @return 移动方向的单位向量
         */
        glm::vec2 GetMovementNormalizeVec2();

    private:
        ConfigManager& m_config_manager = ConfigManager::GetInstance();
};

#endif //ESCAPE_FROM_LILY_KEYBOARDINPUT_H
