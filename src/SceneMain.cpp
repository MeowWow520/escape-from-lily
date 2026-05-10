//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;
    // 设置相机初始位置 应为世界的中心
    m_camera_pos = (m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2) ;
    m_objects = std::vector<Object*>();
    m_player_position = glm::vec2(0.0f, 0.0f);
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

