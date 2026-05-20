//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"

#include "core/Logger/Log.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;
    // TODO: 玩家在世界的中间
    // m_player_position = m_camera_pos + m_game_instance.GetWindowSize() / glm::vec2(2);

    // TODO: 使用工厂方法重构

    m_camera = new Camera();
    EFL_CHECK(LogCategory::Entity, !m_camera->Initialize(), "Camera Initialize()");
    m_camera->SetWorldPos((m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2));


    // 初始化背景
    m_current_background = new Background();
    m_current_background->SetPath("assets/images/backgrounds/purple.png");
    EFL_CHECK(LogCategory::Entity, !m_current_background->Initialize(), "Background Initialize()");
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
    // 删除背景资源
    EFL_CHECK(LogCategory::Core, m_current_background->Quit() == 0, "Background Quit()");
    delete m_current_background;
    // 删除相机资源
    delete m_camera;

    return 0;
}

Camera* SceneMain::GetCamera() {
    return m_camera;
}