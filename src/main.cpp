//
// Created by MeowWow520 on 2026/5/8.
//

#include "core/Def.h"
#include "core/Game.h"



int main() {
    Game& game_instance = Game::GetInstance();
    if (const ssl loc = ssl::current();
        EFL_ClassInit(game_instance.Initialize(), loc) != 0)
        return -1;
    return game_instance.Running();
}
