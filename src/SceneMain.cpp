//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    world_scale_ = glm::vec2{3, 3};
    world_size_ = game_instance_.GetWindowSize() * world_scale_;
    // 设置相机初始位置
    camera_pos_ = (world_size_ - game_instance_.GetWindowSize()) / glm::vec2(2) ;
    objects_ = std::vector<Object*>();
    player_position_ = glm::vec2(0.0f, 0.0f);
    SDL_Log("[core] Initialized SceneMain");
    return 0;
}

void SceneMain::HandleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Update(float dt) {
    (void)dt;
}

void SceneMain::Render() {

}

int SceneMain::Quit() {
    SDL_Log("[core] Quitting SceneMain");
    return 0;
}

