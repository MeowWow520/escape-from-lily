//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_OBJECT_H
#define ESCAPE_FROM_LILY_OBJECT_H
#include "Game.h"



class Object {
    protected:
        Game& m_game_instance = Game::GetInstance();
    public:
        Object() = default;
        virtual ~Object() = default;
        virtual int Initialize() { return 0; }
        virtual void HandleEvents([[maybe_unused]]SDL_Event event) { }
        virtual void Update([[maybe_unused]] const float dt) { }
        virtual void Render() { }
        virtual int Quit() { return 0; }
};

#endif //ESCAPE_FROM_LILY_OBJECT_H
