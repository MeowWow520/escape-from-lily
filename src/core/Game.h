#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
// #include <cerrno>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_mixer.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>




class Scene;



class Game
{
    // 私有成员变量
private:
    bool IsRunning_;
    bool IsLogCurrentFPS;
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
    Game()
    {
        std::ifstream configFile("assets/json/config.json");
        configFile >> Config_;


        IsRunning_ = true;
        IsLogCurrentFPS = Config_["log"]["fps"];
        DeltaTime_ = 0.00f;
        FPS_ = Config_["fps"];
        Title_ = Config_["window"]["title"];
        Window_ = nullptr;
        Renderer_ = nullptr;
        WindowSize_.x = Config_["window"]["size"][0];
        WindowSize_.y = Config_["window"]["size"][1];
        CurrentScene_ = nullptr;

        FrameDelay_ = (Uint64)1e9 / FPS_;
        configFile.close();
    }
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
    int Initialize();      // 初始化游戏
    void handleEvents();   // 处理事件
    void Update(float dt); // 更新游戏状态
    void Render();         // 渲染游戏
    int Clean();           // 清理游戏资源
    static int OutputError();
public:
    glm::vec2 Game::getWindowSize() const { return WindowSize_; }
    bool Game::getIsRunning() const { return IsRunning_; }
    SDL_Window* Game::getWindow() const { return Window_; }
    SDL_Renderer* Game::getRenderer() const { return Renderer_; }
    Scene* Game::getCurrentScene() const { return CurrentScene_; }
public:
    void drawGrid(const glm::vec2& x1, const glm::vec2& x2,
        const size_t gridwidth, SDL_FColor color);
    void drawBoundary(const glm::vec2& x1, const glm::vec2& x2,
        const size_t boundarywidth, SDL_FColor color);
};


#endif // GAME_H    