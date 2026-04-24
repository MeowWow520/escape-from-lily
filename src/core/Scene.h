#ifndef SCENE_H
#define SCENE_H
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Object.h"



class Scene: public Object
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Initialize() override { }
    virtual void handleEvents([[maybe_unused]] SDL_Event& event) override { }
    virtual void Update([[maybe_unused]] float dt) override { }
    virtual void Render() override { }
    virtual void Clean() override { }
};



#endif //! SCENE_H