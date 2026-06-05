//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"
#include "core/Config/ConfigManager.h"
#include "core/Logger/Log.h"


int main() {
    EFL::RegisterLogCategory();
    // 创建 Game 单例
    Game& game_instance = Game::GetInstance();
    if (game_instance.Initialize() != 0) return -1;
    game_instance.Running();
    EFL::QuitLogger();
    return 0;
}
