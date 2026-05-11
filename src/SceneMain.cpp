//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{1, 1};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;
    // 设置相机初始位置 应为世界的中心
    m_camera_pos = (m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2);
    m_player_position = glm::vec2(0.0f, 0.0f);

    /*// 创建背景
    if (auto [it, inserted] = m_texturedentitys.try_emplace("Background", nullptr);
        !inserted) {
            SDL_Log("[SceneMain] TexturedEntity - Background already have, try emplace");
            if (m_texturedentitys["Background"]->Quit() != 0)
                return -1;
        }
    else {
        m_texturedentitys.insert(std::make_pair("Background", new TexturedEntity()));
        m_texturedentitys["Background"]->Initialize();
        SDL_Log("[SceneMain] TexturedEntity - Background created");
    }
    // 初始化背景*/

    if (!m_current_background->InitializeTextureFromPath("assets/images/test_backgrd.jpg"))
        return -1;
    m_current_background->SetWorldPos(glm::vec2{0,0});
    SDL_Log("[SceneMain] Initialized SceneMain");
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
    SDL_Log("[SceneMain] Quitting SceneMain");
    return 0;
}

