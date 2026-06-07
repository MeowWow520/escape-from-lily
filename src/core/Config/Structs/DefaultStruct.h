//
// Created by MeowWow520 on 2026/6/5.
//

#ifndef ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#define ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL_stdinc.h>



struct s_window {
    glm::vec2 size{};
    std::string title;
};

struct s_display {
    Uint32 fps{};
    s_window window{};
};

struct s_feature {
    bool acceleration{};
    bool key_logging{};
};

struct s_font {
    std::string default_color{};
    std::string notosansc{};
};

struct DefaultJson {
    s_display display{};
    s_feature feature{};
    s_font font{};
};

#endif //ESCAPE_FROM_LILY_DEFAULTSTRUCT_H
