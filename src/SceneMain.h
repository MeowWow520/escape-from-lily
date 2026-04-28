#ifndef SCENEMAIN_H
#define SCENEMAIN_H
#include "core/Scene.h"



class SceneMain: public Scene
{
    glm::vec2 WorldSize_;
    glm::vec2 PlayerPos_;
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;

    virtual void Initialize() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void Update(float dt) override;
    virtual void Render() override;
    virtual void Clean() override;


public:
    void renderBackground();
};

#endif //! SCENEMAIN_H
