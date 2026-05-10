//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENE_H
#define ESCAPE_FROM_LILY_SCENE_H

#include "Object.h"



class Scene : public Object {
    protected:
        glm::vec2 camera_pos_{};       // 相机位置
        std::vector<Object*> objects_; // 场景中的物体
    public:
        Scene() = default;
        ~Scene() override = default;
        int Initialize() override { return 0; }
        void HandleEvents(SDL_Event event) override { }
        void Update(float dt) override { }
        void Render() override { }
        int Quit() override { return 0; }
};

#endif //ESCAPE_FROM_LILY_SCENE_H
