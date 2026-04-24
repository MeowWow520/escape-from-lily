#ifndef SCENEMAIN_H
#define SCENEMAIN_H
#include "core/Scene.h"



class SceneMain: public Scene
{
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;

    virtual void Initialize() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void Update(float dt) override;
    virtual void Render() override;
    virtual void Clean() override;
};

#endif //! SCENEMAIN_H
