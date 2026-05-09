//
// Created by MeowWow520 on 2026/5/8.
//

#include "Game.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>


Game::Game() {
   title_ = "escape-from-lily";
   window_size_ = {1960, 1080};
   running_ = true;
   delta_time_ = 0.0f;
   frame_delay_ = 0.0f;
   FPS_ = 120;
   window_ = nullptr;
   renderer_ = nullptr;
   current_scene_ = nullptr;
   SDL_Log("[core] Private value initialized successfully");
}

int Game::Initialize() {


   // 初始化 SDL 库
   if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
      SDL_Log("[core] SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("[core] SDL initialized successfully");
   if (!TTF_Init()) {
      SDL_Log("[core] SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("[core] SDL initialized successfully");
   if (!MIX_Init()) {
      SDL_Log("[core] SDL initialized fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("[core] SDL initialized successfully");

   // 创建窗口
   window_ = SDL_CreateWindow(
      title_.c_str(),
      static_cast<int>(window_size_.x), static_cast<int>(window_size_.y),
      SDL_WINDOW_RESIZABLE);
   if (window_ == nullptr) {
      SDL_Log("[core] SDL_CreateWindow fail: %s", SDL_GetError());
      return -1;
   } SDL_Log("[core] SDL_CreateWindow successfully");

   // 创建场景
   if (current_scene_ != nullptr) {
      current_scene_->Quit();
      delete current_scene_;
   }
   // current_scene_ = new SceneMain();
   // current_scene_->Initialize();
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
            SDL_Log("[core] HandleEvent received SDL_EVENT_QUIT");
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
void Game::Render() const {
   SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
   SDL_RenderClear(renderer_);
   current_scene_->Render();
   SDL_RenderPresent(renderer_);
}

void Game::Quit() {
   SDL_Log("[core] Cleaning Game class");
   SDL_Quit();
   TTF_Quit();
   MIX_Quit();
}
