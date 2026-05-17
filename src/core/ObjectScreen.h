//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_OBJECTSCREEN_H
#define ESCAPE_FROM_LILY_OBJECTSCREEN_H
#include "ObjectWorld.h"


class ObjectScreen : public ObjectWorld {
    protected:
        glm::vec2 m_screen_pos{};
    public:
        ObjectScreen() = default;
        ~ObjectScreen() override = default;

        /**
         * 获取屏幕坐标
         * @return 屏幕坐标
         */
        [[nodiscard]] glm::vec2 GetScreenPos() const { return m_screen_pos; }
        /**
         * 设置屏幕坐标
         * @param new_pos 新的屏幕坐标
         * @return 设置后的屏幕坐标
         */
        glm::vec2 SetScreenPos(const glm::vec2& new_pos) {
            m_screen_pos = new_pos;
            return new_pos;
        }
};

#endif //ESCAPE_FROM_LILY_OBJECTSCREEN_H
