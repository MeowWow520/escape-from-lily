//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENE_H
#define ESCAPE_FROM_LILY_SCENE_H
#include "Object.h"



class Scene : public Object {
    protected:
        glm::vec2 m_world_size{};       // 世界的大小
        glm::vec2 m_camera_pos{};       // 相机位置
    public:
        Scene() = default;
        ~Scene() override = default;
        int Initialize() override { return 0; }
        void HandleEvents(SDL_Event event) override { }
        void Update(float dt) override { }
        void Render() override { }
        int Quit() override { return 0; }

        // setter 和 getter
        [[nodiscard]] glm::vec2 GetWorldSize() const {
             return m_world_size;
        }
        glm::vec2 SetCameraPos(const glm::vec2 newcamerapos) {
            m_camera_pos = newcamerapos;
            return newcamerapos;
        }
        [[nodiscard]] glm::vec2 GetCameraPos() const {
            return m_camera_pos;
        }

};

#endif //ESCAPE_FROM_LILY_SCENE_H
