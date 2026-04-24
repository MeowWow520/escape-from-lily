#ifndef OBJECT_H
#define OBJECT_H
#include "Game.h"



class Object
{
protected:
    Game& gameInstance_ = Game::getInstance();

public:
    Object() = default;
    virtual ~Object() = default;

    virtual void Initialize() { }
    virtual void handleEvents(SDL_Event& event) { }
    virtual void Update(float dt) { }
    virtual void Render() { }
    virtual void Clean() { }
};


#endif //! OBJECT_H
