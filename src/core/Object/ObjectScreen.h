//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_OBJECTSCREEN_H
#define ESCAPE_FROM_LILY_OBJECTSCREEN_H
#include "ObjectWorld.h"


class ObjectScreen : public ObjectWorld {
    protected:
        glm::vec2 m_vec2_screenPos{};
    public:
        explicit ObjectScreen(const char* m_entity_name = "ObjectScreen")
            : ObjectWorld(m_entity_name) {}

        ~ObjectScreen() override = default;

        /**
         * 获取屏幕坐标
         * @return 屏幕坐标
         */
        [[nodiscard]] glm::vec2 getScreenPos() const { return m_vec2_screenPos; }
        /**
         * 设置屏幕坐标
         * @param new_pos 新的屏幕坐标
         * @return 设置后的屏幕坐标
         */
        glm::vec2 setScreenPos(const glm::vec2& new_pos) {
            m_vec2_screenPos = new_pos;
            return new_pos;
        }
};

#endif //ESCAPE_FROM_LILY_OBJECTSCREEN_H
