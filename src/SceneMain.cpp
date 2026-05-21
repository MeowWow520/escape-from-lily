//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"

#include "core/Factory/EntityFactory.h"
#include "core/Logger/Log.h"


int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;
    // TODO: 玩家在世界的中间
    // m_player_position = m_camera_pos + m_game_instance.GetWindowSize() / glm::vec2(2);

    CameraParams camera = {
        (m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2),
        100.0f
    };
    m_camera = EntityFactory::CreateCamera(camera);

    BackgroundParams background = {
        "assets/images/backgrounds/purple.png",
        {0,0}
    };
    m_current_background = EntityFactory::CreateBackground(background);
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
    m_camera.reset();
    m_current_background.reset();
    return 0;
}

Camera* SceneMain::GetCamera() {
    return m_camera.get();
}