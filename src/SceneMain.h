//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENEMAIN_H
#define ESCAPE_FROM_LILY_SCENEMAIN_H
#include "core/Scene.h"
#include "core/Entities/Background.h"
#include "core/Input/KeyboardInput.h"
#include "core/Entities/Camera.h"



class SceneMain : public Scene {
    protected:
        glm::vec2 m_player_position{}; // 玩家的位置
    public:
        SceneMain() = default;
        ~SceneMain() override = default;
        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;
        // getter 和 setter
        glm::vec2 SetPlayerPosition(glm::vec2 newplayerpos);
        [[nodiscard]] glm::vec2 GetPlayerPosition() const;
        [[nodiscard]] glm::vec2 GetWorldScale() const;
    private:
        glm::vec2 m_world_scale{};
        // TODO: 添加背景转换
        Background *m_current_background{};
        Camera *m_camera{};
};

#endif //ESCAPE_FROM_LILY_SCENEMAIN_H
