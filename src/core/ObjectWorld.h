//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_OBJECTWORLD_H
#define ESCAPE_FROM_LILY_OBJECTWORLD_H

#include "Object.h"



class ObjectWorld : public Object {
    protected:
        glm::vec2 m_world_pos{};
    public:
        ObjectWorld() = default;
        ~ObjectWorld() override = default;

        /**
         * 获取世界坐标
         * @return 世界坐标
         */
        [[nodiscard]] glm::vec2 GetWorldPos() const {
            return m_world_pos;
        }
        /**
         * 设置世界坐标
         * @param new_pos 新的世界坐标
         * @return 设置后的世界坐标
         */
        glm::vec2 SetWorldPos(const glm::vec2& new_pos) {
            m_world_pos = new_pos;
            return new_pos;
        }
};


#endif //ESCAPE_FROM_LILY_OBJECTWORLD_H
