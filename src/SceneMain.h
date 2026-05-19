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
    public:
        explicit SceneMain() : Scene("SceneMain") {}

        ~SceneMain() override = default;
        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;

        [[nodiscard]] Camera* GetCamera() override;

    private:
        // TODO: 添加背景转换
        Background *m_current_background{};
        Camera *m_camera{};
};

#endif //ESCAPE_FROM_LILY_SCENEMAIN_H
