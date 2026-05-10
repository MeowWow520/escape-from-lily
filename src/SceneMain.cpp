//
// Created by MeowWow520 on 2026/5/9.
//

#include "SceneMain.h"


int SceneMain::Initialize() {
    camera_pos_ = glm::vec2(0.0f, 0.0f);
    objects_ = std::vector<Object*>();
    SDL_Log("[core] Initialized SceneMain");
    return 0;
}

void SceneMain::HandleEvents(SDL_Event event) {
    (void)event;
}

void SceneMain::Update(float dt) {
    (void)dt;
}

void SceneMain::Render() {

}

int SceneMain::Quit() {
    SDL_Log("[core] Quitting SceneMain");
    return 0;
}
