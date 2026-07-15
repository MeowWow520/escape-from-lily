//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"

#include "core/Factory/EntityFactory.h"
#include "core/Logger/Log.h"



int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_vec2_worldScale = glm::vec2{3, 3};
    m_vec2_worldSize = m_game_instance.getWindowSize() * m_vec2_worldScale;

    // 创建相机
    m_camera = EntityFactory::createCamera(CameraParams{});
    if (!m_camera) {
        EFL_LOGGER_ERROR(LogCategory::Scene, "SceneMain Initialize: camera creation failed");
        return -1;
    }

    // 创建玩家
    m_player = EntityFactory::createPlayer(PlayerParams{
        .player_name = std::string{"MeowWow520"},
        .texture_path = std::string{"assets/images/Entity/Player/Kawaii_Ghost_Sprite/Default.png"},
        .health = 5.00f,
    });
    if (!m_player) {
        EFL_LOGGER_ERROR(LogCategory::Scene, "SceneMain Initialize: player creation failed");
        return -1;
    }

    // 创建背景
    m_current_background = EntityFactory::createBackground(BackgroundParams{
        .texture_path = std::string{"assets/images/backgrounds/purple.png"},
    });
    if (!m_current_background) {
        EFL_LOGGER_ERROR(LogCategory::Scene, "SceneMain Initialize: background creation failed");
        return -1;
    }

    // 创建文字
    m_text_static = EntityFactory::createTextStatic(TextStaticParams{
        .text = std::string{"Hello World"},
    });
    if (!m_text_static) {
        EFL_LOGGER_ERROR(LogCategory::Scene, "SceneMain Initialize: text static creation failed");
        return -1;
    }

    return 0;
}

void SceneMain::Update(const float dt) {
    m_camera->Update(dt);
    m_current_background->Update(dt);
    m_player->Update(dt);
    m_text_static->Update(dt);
}

void SceneMain::handleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Render() {
    m_current_background->Render();
    m_player->Render();
    m_text_static->Render();
}

int SceneMain::Quit() {
    m_camera.reset();
    m_current_background.reset();
    m_player.reset();
    m_text_static.reset();
    return 0;
}

Camera* SceneMain::GetCamera() {
    return m_camera.get();
}

Player * SceneMain::GetPlayer() {
    return m_player.get();
}

