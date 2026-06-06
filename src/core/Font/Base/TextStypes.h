//
// Created by MeowWow520 on 2026/5/27.
//

#ifndef ESCAPE_FROM_LILY_TEXTSTYPES_H
#define ESCAPE_FROM_LILY_TEXTSTYPES_H
#include <string>
#include <SDL3/SDL_pixels.h>
#include "../../Def.h"



enum class TextStype : uint8_t {
    Static,
    Colorful
};

struct BaseString {
    std::string text;      // 文本
    TextStype text_stype;  // 样式
    float     font_size;   // 字体大小
    std::string font_path; // 字体资源
    SDL_Color color;       // 颜色
    float     rotation;    // 旋转
    glm::vec2 pivot;       // 旋转中心
    glm::vec2 scale;       // 缩放 支持上下和左右
    float display_time;    // 展示时间 -1 == infinite
    bool is_display_time_out; // 展示时间超时
};


#endif //ESCAPE_FROM_LILY_TEXTSTYPES_H
