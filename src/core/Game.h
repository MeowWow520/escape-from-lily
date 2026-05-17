//
// Created by MeowWow520 on 2026/5/8.
//

#ifndef ESCAPE_FROM_LILY_GAME_H
#define ESCAPE_FROM_LILY_GAME_H
#include <string>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "Input/KeyboardInput.h"



class Scene;

class Game {
    public:
        // 获取单例
        static Game& GetInstance() {
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
        void Update(float dt) const;
        void Render() const;
        [[nodiscard]] int Quit();

        // Getter 和 Setter
        [[nodiscard]] glm::vec2 GetWindowSize() const;
        [[nodiscard]] SDL_Window *GetSDLWindow() const;
        [[nodiscard]] SDL_Renderer *GetSDLRenderer() const;
        [[nodiscard]] Scene* GetCurrentScene() const;
        [[nodiscard]] KeyboardInput* GetKeyboardInput() const;
    private:
        Game();

        std::string m_title{};
        glm::vec2 m_window_size{};
        bool m_running{};
        float m_delta_time{};
        Uint32 m_frame_delay{};
        Uint32 m_FPS{};
        SDL_Window* m_window{};
        SDL_Renderer* m_renderer{};
        Scene* m_current_scene{};
        int m_return_code{};
        KeyboardInput *m_key_input{};
};

#endif //ESCAPE_FROM_LILY_GAME_H
