//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_INPUT_H
#define ESCAPE_FROM_LILY_INPUT_H
#include "Action.h"

#include <unordered_map>
#include <SDL3/SDL_events.h>


class Input {
    protected:
        // 按键绑定映射表
        std::unordered_map<SDL_Keycode, Action> m_key_bind;
        // 帧数动作状态映射表
        std::unordered_map<Action, ActionState> m_current_action_state;
        std::unordered_map<Action, ActionState> m_preframe_action_state;
        int m_return_code{};
    public:
        Input();
        virtual ~Input() = default;
        // Input(const Input&) = delete;
        // Input& operator=(const Input&) = delete;

        virtual void HandleEvents(SDL_Event event) = 0;
        virtual void Update(float dt);

        // 查询指定动作的原始状态
        [[nodiscard]] ActionState GetActionState(Action action) const;

        // 本帧刚按下？
        [[nodiscard]] bool IsActionPressed(Action action) const;
        // 持续按住中？
        [[nodiscard]] bool IsActionHeld(Action action) const;
        // 本帧刚松开？
        [[nodiscard]] bool IsActionReleased(Action action) const;

        /**
         * BindAction 绑定按键到动作
         *
         * @param key 原始按键绑定
         * @param action 对应功能的动作
         * @return 绑定成功返回 true，失败返回 false
         */
        bool BindAction(SDL_Keycode key, Action action);

        /**
         * UnbindAction 解绑 xxx 动作
         * @param action 要解绑的动作
         * @return 绑定成功返回 true，失败返回 false
         */
        bool UnbindAction(Action action);

    protected:
        // 设置默认按键映射
        virtual bool SetDefaultKeyBind();

        /**
         * SetActionState 设置 xxx 动作的当前状态。
         * @param action 要设置的动作
         * @param state 当前状态
         */
        void SetActionState(Action action, ActionState state);
    };

#endif //ESCAPE_FROM_LILY_INPUT_H
