#define CRT_SECURE_NO_WARNINGS
#define SDL_MAIN_HANDLED
#include "core/Game.h"



int main(int argc, char *argv[])
{
    SDL_Log("Hello escape-from-lily!");
    Game& gameInstance = Game::getInstance();
    SDL_Log("gameInstance creeted");
    if (gameInstance.Initialize() == -1) return -1;
    return gameInstance.Running(argc, argv);
}