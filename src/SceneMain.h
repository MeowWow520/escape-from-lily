//
// Created by MeowWow520 on 2026/5/9.
//

#ifndef ESCAPE_FROM_LILY_SCENEMAIN_H
#define ESCAPE_FROM_LILY_SCENEMAIN_H
#include <map>
#include "core/Scene.h"
#include "core/Entities/Background.h"


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

        // FIXME: getter 和 setter 是否有必要？
        /**
         * [[nodiscard]] glm::vec2 GetPlayerPosition() const {
         *     return player_position_;
         * }
         */

    private:
        glm::vec2 m_world_scale{};
        Background *m_current_background{};
};

#endif //ESCAPE_FROM_LILY_SCENEMAIN_H
