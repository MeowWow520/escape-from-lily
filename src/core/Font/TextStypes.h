//
// Created by MeowWow520 on 2026/5/23.
//

#ifndef ESCAPE_FROM_LILY_TEXTSTYPES_H
#define ESCAPE_FROM_LILY_TEXTSTYPES_H
#include <SDL3/SDL_pixels.h>
#include "glm/vec2.hpp"

enum class TextStype : uint8_t {
    Static,  // 静态显示
    Dynamic  // 动态
};

struct TextParam {
    TextStype text_stype;        // 样式
    int text_display_time;       /// 展示的帧数 1s = (int) FPS 帧数 -1 展示无限时长
    float font_size;             // 字号
    size_t text_width;           // 文本长度
    size_t text_each_line_width; // 每一行最长的文本数
    SDL_Color color;             // 文本颜色
    float line_spacing;          // 行间距
    std::string font_path;       // 使用的字体
    bool display_backgrd;        // 是否启用背景色？
    SDL_Color back_color;        /// 背景色
    float rotation;              // 旋转角度
    glm::vec2 scale;             // 缩放
    glm::vec2 pivot;             // 旋转中心 归一化
};

struct StringParam {
    std::string text;         /// 文本
    bool is_per_char;         // 是否分词渲染
    char* current_char;       // 当前字符
    TextParam text_param;     // 字符样式
};
#endif //ESCAPE_FROM_LILY_TEXTSTYPES_H
