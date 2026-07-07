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
        static Game& getInstance() {
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
        void handleEvents();
        void Update(float dt) const;
        void Render() const;

        /**
         * 清理游戏资源
         *
         * @return 清理成功返回 0，失败返回 -1
         */
        int Quit();

        // setter 和 getter
        [[nodiscard]] glm::vec2 getWindowSize() const;
        bool setRunning(bool new_running);
        [[nodiscard]] bool getRunning() const;
        // m_delta_time
        [[nodiscard]] Uint32 getFPS() const;
        [[nodiscard]] SDL_Window *getSDLWindow() const;
        [[nodiscard]] SDL_Renderer *getSDLRenderer() const;
        [[nodiscard]] Scene* getCurrentScene() const;
        [[nodiscard]] KeyboardInput* getKeyboardInput() const;

    private:
        FontManager& font_manager;
        ConfigManager& config_manager;
        Game();

        std::string m_strTitle{};
        glm::vec2 m_vec2_windowSize{};
        bool m_bRunning{};
        float m_fDeltaTime{};
        Uint32 m_i32FrameDelay{};
        Uint32 m_i32FPS{};
        SDL_Window* m_window{};
        SDL_Renderer* m_renderer{};
        Scene* m_currentScene{};
        KeyboardInput *m_keyInput{};
};

#endif //ESCAPE_FROM_LILY_GAME_H
