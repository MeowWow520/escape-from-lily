//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_OBJECTWORLD_H
#define ESCAPE_FROM_LILY_OBJECTWORLD_H

#include "Object.h"



class ObjectWorld : public Object {
    protected:
        glm::vec2 world_pos_{};
    public:
        ObjectWorld() = default;
        ~ObjectWorld() override = default;
        int Initialize() override { return 0; }
        void HandleEvents(SDL_Event event) override { }
        void Update(float dt) override { }
        void Render() override { }
        int Quit() override { return 0; }

        /**
         * 获取世界坐标
         * @return 世界坐标
         */
        [[nodiscard]] glm::vec2 GetWorldPos() const {
            return world_pos_;
        }
        /**
         * 设置世界坐标
         * @param new_pos 新的世界坐标
         * @return 设置后的世界坐标
         */
        glm::vec2 SetWorldPos(const glm::vec2& new_pos) {
            world_pos_ = new_pos;
            return new_pos;
        }
};


#endif //ESCAPE_FROM_LILY_OBJECTWORLD_H
