//
// Created by MeowWow520 on 2026/6/5.
//

#ifndef ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#define ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL.h>


struct s_WindowParams {
    std::string title;
    glm::vec2 window_size;
    int FPS;
};

struct s_PlayerParams {
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
struct s_CameraParams {
    std::string name;
};

struct s_FeatureParams {
    bool key_logging;
    bool acceleration;
};

struct DefaultJson {
    s_WindowParams window_params;
    s_PlayerParams player_params;
    s_CameraParams camera_params;
    s_FeatureParams feature;
};

#endif //ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
