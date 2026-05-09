//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"

int main() {
    Game& game_instance = Game::GetInstance();
    if (game_instance.Initialize() == -1)
        SDL_Log("[core] Game was unable to initialize. return -1");
    return game_instance.Running();
}