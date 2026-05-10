//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENEMAIN_H
#define ESCAPE_FROM_LILY_SCENEMAIN_H
#include "core/Scene.h"


class SceneMain : public Scene {

        glm::vec2 world_size_;      // 世界的大小
        glm::vec2 player_position_; // 玩家的位置
    public:
        SceneMain() = default;
        ~SceneMain() override = default;
        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;
};

#endif //ESCAPE_FROM_LILY_SCENEMAIN_H
