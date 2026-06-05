//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"
#include "core/Config/ConfigManager.h"
#include "core/Logger/Log.h"


int main() {
    EFL::RegisterLogCategory();
    // 创建 Config 单例
    ConfigManager& config_manager = ConfigManager::GetInstance();
    if (config_manager.Initialize() != 0) return -1;
    // 创建 Game 单例
    Game& game_instance = Game::GetInstance();
    if (game_instance.Initialize() != 0) return -1;

    game_instance.Running();
    if (config_manager.Quit() != 0) return -1;
    EFL::QuitLogger();

    return 0;
}
