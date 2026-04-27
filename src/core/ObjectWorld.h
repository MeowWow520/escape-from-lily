#ifndef OBJECTWORLD_H
#define OBJECTWORLD_H
#include <glm/glm.hpp>
#include "Object.h"



class ObjectWorld: public Object
{
protected:
    glm::vec2 WorldPos_;
public:
    ObjectWorld() = default;
    virtual ~ObjectWorld() = default;
    virtual void Initialize() override { }
    virtual void handleEvents([[maybe_unused]] SDL_Event& event) override { }
    virtual void Update([[maybe_unused]] float dt) override { }
    virtual void Render() override { }
    virtual void Clean() override { }


    glm::vec2 getWorldPos() const { return WorldPos_; };
    void setWorldPos(const glm::vec2& newpos) { WorldPos_ = newpos; };

};
#endif //!OBJECTWORLD_H