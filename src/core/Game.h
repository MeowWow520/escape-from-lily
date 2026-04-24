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
    // 变量
    std::string title_;
    glm::vec2 windowSize_;
    bool isRunning_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    float deltaTime_;
    // 帧延迟
    Uint64 frameDelay_;
    Uint64 FPS_;
    Scene* currentScene_;

private:
    // 私有构造函数
    Game() { }
    // 禁止拷贝构造函数与赋值操作符
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    // 单例模式示例获取
    static Game& Game::getInstance()
    {
        static Game instance;
        return instance;
    }
    /**
     * 运行主循环
     * @return 返回 0：成功；返回 -1：运行失败。
     */
    int Running(int argc, char *argv[]);
    /**
     * 初始化资源，初始化 SDL 字体库、图片库、音效库。
     * @return 返回 0：成功；返回 -1：初始化失败。
     */   
    int Initialize(); // 初始化游戏
    void HandleEvents(); // 处理事件
    void Update(float dt); // 更新游戏状态
    void Render(); // 渲染游戏
    int Clean(); // 清理游戏资源


    glm::vec2 getWindowSize() const { return windowSize_; }
    bool getIsRunning() const { return isRunning_; }
    SDL_Window* getWindow() { return window_; }
    SDL_Renderer* getRenderer() { return renderer_; }
    Scene* getCurrentScene() { return currentScene_; }

};


#endif // GAME_H    