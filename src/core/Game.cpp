#include "Game.h"



int Game::Running(int argc, char *argv[])
{
    (void)argc, (void)argv;

    auto FrameTime = (1e9 / FPS_);
    auto DeltaTime = 0.0f;
    while (IsRunning_)
    {
        // 计算帧率延迟
        Uint64 start = SDL_GetTicksNS();

        //<!-- START 逻辑处理
        handleEvents();
        Update(DeltaTime_);
        Render();
        //<!-- END 逻辑处理

        // 计算帧率
        {
            Uint64 end = SDL_GetTicksNS();
            Uint64 elapsed = end - start;
            if (elapsed < FrameDelay_) {
                Uint64 sdlDelatTimesNS = FrameDelay_ - elapsed;
                SDL_DelayNS(sdlDelatTimesNS);
                DeltaTime_ = (float)(FrameDelay_ / 1e9);
            } else { DeltaTime_ = (float)(elapsed / 1e9); }
        }
    }

    return 0;
}

int Game::Initialize()
{
    
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) return OutputError();
    if (!TTF_Init()) return OutputError();
    if (!MIX_Init()) return OutputError(); 

    Window_ = SDL_CreateWindow(Title_.c_str(), static_cast<int>(WindowSize_.x), static_cast<int>(WindowSize_.y), SDL_WINDOW_RESIZABLE);
    if (Window_ == nullptr) return OutputError();
    
    Renderer_ = SDL_CreateRenderer(Window_, NULL);
    if (Renderer_ == nullptr) return OutputError();
    
    bool SDLSetRLP = SDL_SetRenderLogicalPresentation(Renderer_, static_cast<int>(WindowSize_.x), static_cast<int>(WindowSize_.y), SDL_LOGICAL_PRESENTATION_LETTERBOX);
    if (!SDLSetRLP) return OutputError();
    
    // Create Scene
    return 0;
}

void Game::handleEvents()
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
}

int Game::Clean()
{
    return 0;
}

int Game::OutputError()
{
    SDL_Log(SDL_GetError());
    return -1;
}