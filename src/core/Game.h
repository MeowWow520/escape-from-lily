//
// Created by MeowWow520 on 2026/5/8.
//

#ifndef ESCAPE_FROM_LILY_GAME_H
#define ESCAPE_FROM_LILY_GAME_H
#include <string>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "Config/ConfigManager.h"
#include "Input/KeyboardInput.h"



class Scene;
class FontManager;


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
         * 初始化游戏资源，包含：SDL 库、相机、背景、各个实体的初始化
         *
         * @return 初始化成功返回 0，失败返回 -1
         */
        int Initialize();
        /**
         * 运行游戏
         * @return 运行成功返回 0，失败返回 -1
         */
        int Running();
        void HandleEvents();
        void Update(float dt) const;
        void Render() const;

        /**
         * 清理游戏资源
         *
         * @return 清理成功返回 0，失败返回 -1
         */
        int Quit();

        // setter 和 getter
        [[nodiscard]] glm::vec2 GetWindowSize() const;
        bool SetRunning(bool newrunning);
        [[nodiscard]] bool GetRunning() const;
        // m_delta_time
        [[nodiscard]] Uint32 GetFPS() const;
        [[nodiscard]] SDL_Window *GetSDLWindow() const;
        [[nodiscard]] SDL_Renderer *GetSDLRenderer() const;
        [[nodiscard]] Scene* GetCurrentScene() const;
        [[nodiscard]] KeyboardInput* GetKeyboardInput() const;

    private:
        FontManager& font_manager;
        ConfigManager& config_manager;
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
