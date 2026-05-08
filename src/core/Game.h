//
// Created by MeowWow520 on 2026/5/8.
//

#ifndef ESCAPE_FROM_LILY_GAME_H
#define ESCAPE_FROM_LILY_GAME_H
#include <SDL3/SDL.h>
// TODO: Is here useful?
// #include <SDL3_ttf/SDL_ttf.h>
// #include <SDL3_image/SDL_image.h>
// #include <SDL3_mixer/SDL_mixer.h>
#include <glm/glm.hpp>


class Game {
    public:
        // 获取单例
        static Game& getInstance() {
            static Game instance;
            return instance;
        }
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        /**
         * 初始化 SDL 库
         * @return -1 -- 初始化库失败 || 0 -- 无异常
         */
        int Initialize();
        /**
         * 运行游戏
         * @return 0 -- 成功 || -1 -- 异常
         */
        int Running();
        void HandleEvents();

        static void Update(float dt);
        void Render();
        static void Quit();
    private:
        Game();

        glm::vec2 window_size_{};
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        bool running_;
        float delta_time_;
        Uint32 frame_delay_;
        Uint32 FPS_;

};

#endif //ESCAPE_FROM_LILY_GAME_H
