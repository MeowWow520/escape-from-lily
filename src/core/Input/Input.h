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
        // 检查函数的返回值
        int m_return_code{};
    public:
        Input();
        virtual ~Input() = default;
        // Input(const Input&) = delete;
        // Input& operator=(const Input&) = delete;
        /**
         * HandleEvents 通过处理 SDL_Event 来获取那、哪些按键被按下，哪些松开
         *
         * @param event 要处理的 SDL 事件系统
         */
        virtual void HandleEvents(SDL_Event event) = 0;

        /**
         * Update 更新 m_current_action_state 表格：
         * Idle --> Idle || Pressed --> Held || Held --> Held || Released --> Idle
         *
         * @param dt delta time
         */
        virtual void Update(float dt);

        /**
         * 查询某个动作当前的状态
         *
         * @param action 要查询的动作
         * @return 查询的动作的当前的状态
         */
        [[nodiscard]] ActionState GetActionState(Action action) const;

        /**
         * 返回某一个动作是否被按下
         *
         * @param action 要查询的动作
         * @return 符合条件返回 true，不符合返回 false
         */
        [[nodiscard]] bool IsActionPressed(Action action) const;

        /**
         * 返回某一个动作是否正在按住
         *
         * @param action 要查询的动作
         * @return 符合条件返回 true，不符合返回 false
         */
        [[nodiscard]] bool IsActionHeld(Action action) const;

        /**
         * 返回某个按键是否松开
         *
         * @param action 要查询的动作
         * @return 符合条件返回 true，不符合返回 false
         */
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

        // 设置默认按键映射
        virtual bool SetDefaultKeyBind();
    protected:

        /**
         * SetActionState 设置 xxx 动作的当前状态。
         * @param action 要设置的动作
         * @param state 当前状态
         */
        void SetActionState(Action action, ActionState state);

};

#endif //ESCAPE_FROM_LILY_INPUT_H
