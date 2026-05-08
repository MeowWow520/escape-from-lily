//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Game.h"

int main() {
    Game& gameInstance = Game::getInstance();
    if (gameInstance.Initialize() == -1) {
        SDL_Log("Game was unable to initialize. return -1");
    } return gameInstance.Running();
}