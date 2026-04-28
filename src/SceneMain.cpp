#include "SceneMain.h"



void SceneMain::Initialize()
{
    WorldSize_ = gameInstance_.getWindowSize() * gameInstance_.getWorldScale();
    CameraPos_ = gameInstance_.getDefaultPlayerPos();
}

void SceneMain::handleEvents(SDL_Event& event)
{
    (void)event;
}

void SceneMain::Update(float dt)
{
    CameraPos_ += glm::vec2(10.0f, 10.0f) * dt; // 每秒向右下方移动10个单位
}

void SceneMain::Render()
{
    renderBackground();
}

void SceneMain::Clean()
{

}

void SceneMain::renderBackground()
{
    gameInstance_.drawGrid(- CameraPos_, WorldSize_, 50, SDL_FColor{ 0.2f, 0.2f, 0.2f, 1.0f });
    gameInstance_.drawBoundary(- CameraPos_, WorldSize_, 10, SDL_FColor{ 1.0f, 1.0f, 1.0f, 1.0f });
}
