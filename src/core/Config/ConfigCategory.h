//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGCATEGORY_H
#define ESCAPE_FROM_LILY_CONFIGCATEGORY_H
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL_pixels.h>



struct Display_Window {
    glm::vec2 size;
    std::string title;
};

struct Display {
    float fps;
    Display_Window window;
};

struct Feature {
    bool acceleration;
    bool keylogging;
};

// FIXME: 在 FontManager 中实现
struct Font {
    SDL_Color color;
    std::string default_font_path;
    float font_size;
};



#endif //ESCAPE_FROM_LILY_CONFIGCATEGORY_H
