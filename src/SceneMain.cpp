//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"

#include "core/Factory/EntityFactory.h"
#include "core/Logger/Log.h"
#include "core/Font/EFL_FontDef.h"

int SceneMain::Initialize() {
    // 设置世界缩放 和 世界大小
    m_world_scale = glm::vec2{3, 3};
    m_world_size = m_game_instance.GetWindowSize() * m_world_scale;

    // 创建相机
    CameraParams camera = {
        (m_world_size - m_game_instance.GetWindowSize()) / glm::vec2(2),
         100.0f * m_game_instance.GetCurrentScene()->GetWorldScale().x
    };
    m_camera = EntityFactory::CreateCamera(camera);

    // 创建背景
    BackgroundParams background = {
        "assets/images/backgrounds/purple.png",
        {0,0}
    };
    m_current_background = EntityFactory::CreateBackground(background);

    // 创建玩家
    PlayerParams player = {
        ConfigManager::GetPlayer().name,
        ConfigManager::GetPlayer().texture_path
    };
    m_player = EntityFactory::CreatePlayer(player);

    TextStaticParams text_static_params = {
        "你好, World!",
        DEFAULT_FONT_SIZE,
         DEFAULT_TEXT_COLOR,
        {50.00, 50.00},
        -1,
    };
    m_text_static = EntityFactory::CreateTextStatic(text_static_params);

    return 0;
}

void SceneMain::HandleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Update(const float dt) {
    // FIXME: 顺序重要吗?
    m_camera->Update(dt);
    m_current_background->Update(dt);
    m_player->Update(dt);
    m_text_static->Update(dt);
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

