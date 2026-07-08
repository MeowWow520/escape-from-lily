//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_OBJECT_H
#define ESCAPE_FROM_LILY_OBJECT_H
#include "../Game.h"
#include "../Config/ConfigManager.h"


class Object {
    protected:
        // 获取 Game 的单例
        Game& m_game_instance = Game::getInstance();
        // 获取 ConfigManager 单例
        ConfigManager& m_config_manager = ConfigManager::getInstance();
        // 实例名字，用语日志系统
        const char* m_cEntityName;
    public:
        explicit Object(const char* m_entity_name) noexcept;
        virtual ~Object();

        virtual int Initialize() { return 0; }
        virtual void handleEvents([[maybe_unused]]SDL_Event event) { }
        virtual void Update([[maybe_unused]] const float dt) { }
        virtual void Render() { }
        virtual int Quit() { return 0; }

        [[nodiscard]] const char* getEntityName() const { return m_cEntityName; }
};

#endif //ESCAPE_FROM_LILY_OBJECT_H
