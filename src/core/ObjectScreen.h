//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_OBJECTSCREEN_H
#define ESCAPE_FROM_LILY_OBJECTSCREEN_H

#include "Object.h"


class ObjectScreen : public Object {
    protected:
        glm::vec2 screen_pos_{};
    public:
        ObjectScreen() = default;
        ~ObjectScreen() override = default;
        int Initialize() override { return 0; }
        void HandleEvents(SDL_Event event) override { }
        void Update(float dt) override { }
        void Render() override { }
        void Quit() override { }

        /**
         * 获取屏幕坐标
         * @return 屏幕坐标
         */
        [[nodiscard]] glm::vec2 GetScreenPos() const {
            return screen_pos_;
        }

        /**
         * 设置屏幕坐标
         * @param new_pos 新的屏幕坐标
         * @return 设置后的屏幕坐标
         */
        glm::vec2 SetScreenPos(const glm::vec2& new_pos) {
            screen_pos_ = new_pos;
            return new_pos;
        }
};

#endif //ESCAPE_FROM_LILY_OBJECTSCREEN_H
