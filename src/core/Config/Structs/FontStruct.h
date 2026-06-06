//
// Created by MeowWow520 on 2026/6/6.
//

#ifndef ESCAPE_FROM_LILY_FONTSTRUCT_H
#define ESCAPE_FROM_LILY_FONTSTRUCT_H
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


struct FontJson {
    std::string text;      // 文本
    std::string stype;     // 样式
    float     font_size;   // 字体大小
    std::string font_path; // 字体资源
    glm::vec4 color;       // 颜色
    float     rotation;    // 旋转
    glm::vec2 pivot;       // 旋转中心
    glm::vec2 scale;       // 缩放 支持上下和左右
    float display_time;    // 展示时间 -1 == infinite
    bool is_display_time_out; // 展示时间超时
};


#endif //ESCAPE_FROM_LILY_FONTSTRUCT_H
