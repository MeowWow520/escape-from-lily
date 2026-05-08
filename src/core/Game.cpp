//
// Created by MeowWow520 on 2026/5/8.
//

#include "Game.h"


Game::Game() {
   window_size_ = {1960, 1080};
   running_ = true;
   window_ = nullptr;
   renderer_ = nullptr;
   delta_time_ = 0.0f;
   frame_delay_ = 0.0f;
   FPS_ = 120;
   SDL_Log("Private value initialized successfully");
};

int Game::Initialize() {


   // 初始化 SDL 库
   if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
      SDL_Log("SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("SDL initialized successfully");
   if (!TTF_Init()) {
      SDL_Log("SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("SDL initialized successfully");
   if (!MIX_Init()) {
      SDL_Log("SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("SDL initialized successfully");

   // 创建窗口
   window_ = SDL_CreateWindow(
      "title",
      static_cast<int>(window_size_.x), static_cast<int>(window_size_.y),
      SDL_WINDOW_RESIZABLE);
   if (window_ == nullptr) {
      SDL_Log("SDL_CreateWindow fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("SDL_CreateWindow successfully");



   return 0;
}

int Game::Running() {

   while (running_) {
      const Uint64 start = SDL_GetTicksNS();

      HandleEvents();
      Update(delta_time_);
      Render();

      const Uint64 end = SDL_GetTicksNS();
      const Uint64 elapsed = end - start;
      if (elapsed < frame_delay_) {
         SDL_DelayNS(frame_delay_ - elapsed);
         delta_time_ = static_cast<float>(frame_delay_ / 1e9);
      } delta_time_ = static_cast<float>(static_cast<double>(elapsed) / 1e9);
   }

   Quit();
   return 0;
}

void Game::HandleEvents() {
   SDL_Event event;
   while (SDL_PollEvent(&event)) {
      switch (event.type) {
         case SDL_EVENT_QUIT:
            SDL_Log("handleEvent received SDL_EVENT_QUIT");
            running_ = false;
            break;
         default:
            // CurrentScene_->handleEvents(event);
            break;
      }
   }
}

void Game::Update(float dt) {

}
void Game::Render() {
   SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
   SDL_RenderClear(renderer_);
   // CurrentScene_->Render();
   SDL_RenderPresent(renderer_);
}

void Game::Quit() {
   SDL_Log("Cleaning Game class");
   SDL_Quit();
   TTF_Quit();
   MIX_Quit();
}
