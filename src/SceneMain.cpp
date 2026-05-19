//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;
    // TODO: 在派生类中实现?
    // TODO: 玩家在世界的中间
    // m_player_position = m_camera_pos + m_game_instance.GetWindowSize() / glm::vec2(2);


    // TODO: 使用工厂方法重构

    m_camera = new Camera();
    if (m_camera->Initialize()) return -1;
    m_camera->SetWorldPos((m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2));


    // 初始化背景
    m_current_background = new Background();
    m_current_background->SetPath("assets/images/backgrounds/purple.png");
    if (m_current_background->Initialize()) return -1;
    m_current_background->SetWorldPos(glm::vec2{0,0});

    return 0;
}

void SceneMain::HandleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Update(const float dt) {
    m_camera->Update(dt);
    m_current_background->Update(dt);
}

void SceneMain::Render() {
    m_current_background->Render();
}

int SceneMain::Quit() {
    if (m_current_background->Quit()) return -1;
    // 删除相机资源
    delete m_camera;

    return 0;
}

Camera* SceneMain::GetCamera() {
    return m_camera;
}