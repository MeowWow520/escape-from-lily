//
// Created by MeowWow520 on 2026/6/5.
//

#ifndef ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#define ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL_stdinc.h>

struct j_PlayerParams {
    std::string player_name;
    glm::vec2 scale;
    glm::vec2 pivot;
    float max_speed;
    float acceleration;
    float health;
    bool visible;
    float rotation;
    SDL_Color color;
    std::string blend_mode;
    SDL_Rect rect;
};
struct j_CameraParams {
    std::string name;
};
struct DefaultJson {
    j_PlayerParams player_params;
    j_CameraParams camera_params;
};

#endif //ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
