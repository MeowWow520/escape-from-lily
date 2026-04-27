#include "Game.h"
#include "../SceneMain.h"



int Game::Running(int argc, char *argv[])
{
    (void)argc, (void)argv;

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
        Uint64 end = SDL_GetTicksNS();
        Uint64 elapsed = end - start;
        if (elapsed < FrameDelay_) {
            SDL_DelayNS(FrameDelay_ - elapsed);
            DeltaTime_ = (float) (FrameDelay_ / 1e9);
        } else { DeltaTime_ = (float)(elapsed / 1e9); }
        if (IsLogCurrentFPS) SDL_Log("Current FPS: %f", (1.0f / DeltaTime_));

    }

    if(!IsRunning_) return Clean();
    return -1;
}

int Game::Initialize()
{
    
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) return OutputError();
    if (!TTF_Init()) return OutputError();
    if (!MIX_Init()) return OutputError(); 
    SDL_Log("SDL lib initialized");

    Window_ = SDL_CreateWindow(Title_.c_str(), 
        static_cast<int>(WindowSize_.x), 
        static_cast<int>(WindowSize_.y), 
        SDL_WINDOW_RESIZABLE );
    if (Window_ == nullptr) return OutputError();
    SDL_Log("SDL created window. Game::Window_ been the window");

    Renderer_ = SDL_CreateRenderer(Window_, NULL);
    if (Renderer_ == nullptr) return OutputError();
    SDL_Log("SDL created renderer. Game::Renderer_ been the renderer");

    bool SDLSetRLP = SDL_SetRenderLogicalPresentation(Renderer_,
        static_cast<int>(WindowSize_.x), 
        static_cast<int>(WindowSize_.y), 
        SDL_LOGICAL_PRESENTATION_LETTERBOX );
    if (!SDLSetRLP) return OutputError();
    SDL_Log("Logical presentation set");
    // Create Scene
    if (CurrentScene_ == nullptr)
        CurrentScene_ = new SceneMain();
    CurrentScene_->Initialize();
    
    return 0;
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            SDL_Log("handleEvent recieved SDL_EVENT_QUIT");
            IsRunning_ = false;
            break;
        default:
            CurrentScene_->handleEvents(event);
            break;
        }
    }
}

void Game::Update(float dt)
{
    CurrentScene_->Update(dt);
}

void Game::Render()
{
    SDL_SetRenderDrawColor(Renderer_, 0, 0, 0, 255);
    SDL_RenderClear(Renderer_);
    CurrentScene_->Render();
    SDL_RenderPresent(Renderer_);
}

int Game::Clean()
{
    SDL_Log("Cleaning Game class");
    SDL_Quit();
    TTF_Quit();
    MIX_Quit();
    return 0;
}

int Game::OutputError()
{
    SDL_Log(SDL_GetError());
    return -1;
}