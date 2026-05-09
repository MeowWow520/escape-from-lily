//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_OBJECT_H
#define ESCAPE_FROM_LILY_OBJECT_H
#include "Game.h"



class Object {
    protected:
        Game& gameInstance_ = Game::GetInstance();
    public:
        Object() = default;
        virtual ~Object() = default;
        virtual int Initialize();
        virtual void HandleEvents(SDL_Event event);
        virtual void Update(float dt);
        virtual void Render();
        virtual void Quit();
};

#endif //ESCAPE_FROM_LILY_OBJECT_H
