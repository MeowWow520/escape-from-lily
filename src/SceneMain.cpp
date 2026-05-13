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

    // TODO: 使用工厂方法重构
    // 初始化背景
    m_current_background = new Background();
    m_current_background->SetPath("assets/images/test_backgrd.png");
    if (m_current_background->Initialize()) {
        m_return_code = -1;
        goto to_quit;
    }
    m_current_background->SetWorldPos(glm::vec2{0,0});
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassInit(m_return_code, loc);
}

void SceneMain::HandleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Update(float dt) {
    (void)dt;
}

void SceneMain::Render() {
    m_current_background->Render();
}

int SceneMain::Quit() {
    goto to_quit;
to_quit:
    const ssl loc = ssl::current();
    return EFL_ClassQuit(Scene::Quit(), loc);
}

glm::vec2 SceneMain::SetPlayerPosition(const glm::vec2 newplayerpos) {
    m_player_position = newplayerpos;
    return newplayerpos;
}

glm::vec2 SceneMain::GetPlayerPosition() const {
    return m_player_position;
}

