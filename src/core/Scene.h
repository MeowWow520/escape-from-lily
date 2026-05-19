//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENE_H
#define ESCAPE_FROM_LILY_SCENE_H
#include "Object/Object.h"
#include "Entities/Camera.h"


class Scene : public Object {
    protected:
        glm::vec2 m_world_size{};       // 世界的大小
        glm::vec2 m_world_scale{};
    public:
        Scene() = default;
        ~Scene() override = default;
        int Initialize() override { return 0; }
        void HandleEvents(SDL_Event event) override { }
        void Update(float dt) override { }
        void Render() override { }
        int Quit() override { return 0; }
        [[nodiscard]] virtual Camera* GetCamera() { return nullptr; }

        // setter 和 getter
        [[nodiscard]] glm::vec2 GetWorldSize() const {
             return m_world_size;
        }
        [[nodiscard]] glm::vec2 GetWorldScale() const{
            return m_world_scale;
        }

};

#endif //ESCAPE_FROM_LILY_SCENE_H
