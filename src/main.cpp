//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"



int main() {
    Game& game_instance = Game::GetInstance();
    SDL_Log("[core] game_instance created");
    if (game_instance.Initialize() == -1) {
        SDL_Log("[core] Game initialized fail, return -1");
        return -1;
    }
    return game_instance.Running();
}