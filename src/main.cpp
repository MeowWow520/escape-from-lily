//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"
#include "core/Config/ConfigManager.h"
#include "core/Logger/Log.h"


int main() {
    EFL::RegisterLogCategory();
    // 创建 ConfigManager 单例
    ConfigManager& config_manager = ConfigManager::GetInstance();
    EFL_CHECK(LogCategory::Core, config_manager.Initialize() == 0, "Config manager initialize");
    // 创建 Game 单例
    Game& game_instance = Game::GetInstance();
    EFL_CHECK(LogCategory::Core, game_instance.Initialize() == 0, "Game instance initialize");
    game_instance.Running();
    EFL::QuitLogger();
    return 0;
}
