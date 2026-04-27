#ifndef OBJECTSCREEN_H
#define OBJECTSCREEN_H
#include <glm/glm.hpp>
#include "ObjectWorld.h"


class ObjectScreen: public ObjectWorld
{
protected:
    glm::vec2 ScreenPos_;
public:
    ObjectScreen() = default;
    virtual ~ObjectScreen() = default;
    virtual void Initialize() override { }
    virtual void handleEvents([[maybe_unused]] SDL_Event& event) override { }
    virtual void Update([[maybe_unused]] float dt) override { }
    virtual void Render() override { }
    virtual void Clean() override { }

    glm::vec2 getScreenPos() const { return ScreenPos_; };
    void setScreenPos(const glm::vec2& newpos) { ScreenPos_ = newpos; };
};



#endif //! OBJECTSCREEN_H