#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_mixer.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>




class Scene;


class Game
{
private:
    bool IsRunning_;
    float DeltaTime_;
    Uint64 FrameDelay_;
    Uint64 FPS_;
    std::string Title_;
    SDL_Window* Window_;
    SDL_Renderer* Renderer_;
    glm::vec2 WindowSize_;
    nlohmann::json Config_;
    Scene* CurrentScene_;

private:
    Game() { }
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    // 单例模式示例获取
    static Game& Game::getInstance()
    {
        static Game instance;
        return instance;
    }

private:
    /**
     * 运行主循环
     * @return 返回 0：成功；返回 -1：运行失败。
     */
    int Running(int argc, char *argv[]);
    /**
     * 初始化资源，初始化 SDL 字体库、图片库、音效库。
     * @return 返回 0：成功；返回 -1：初始化失败。
     */   
    int Initialize();      // 初始化游戏
    void handleEvents();   // 处理事件
    void Update(float dt); // 更新游戏状态
    void Render();         // 渲染游戏
    int Clean();           // 清理游戏资源
    static int OutputError();
public:
    glm::vec2 getWindowSize() const { }
    bool getIsRunning() const { }
    SDL_Window* getWindow() const { }
    SDL_Renderer* getRenderer() const { }
    Scene* getCurrentScene() const { }

};


#endif // GAME_H    