//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_DEF_H
#define ESCAPE_FROM_LILY_DEF_H
#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>


/**
 * 将 RGBA 颜色值分开转换为 4 位数值
 *
 * 如 COLOR(0xFF00FF00) --> 255, 0, 255, 0
 * @param HEX RGBA 颜色值
 */
#define COLOR(HEX) (         \
    ( (HEX) >> 24 ) & 0xFF), \
    (((HEX) >> 16 ) & 0xFF), \
    (((HEX) >> 8  ) & 0xFF), \
    ( (HEX)         & 0xFF)

/**
 * EFL_Vec2AddToRect 将 a 和 b 合成为一个 SDL_Rect c。c 的前两个分量为 a 的值，后两个为 b 的值。
 * @param a 其中一个向量
 * @param b 另一个向量
 * @return a 和 b 合成的新向量
 */
inline SDL_Rect EFL_Vec2AddToRect(const glm::vec2 a, const glm::vec2 b) {
    return {static_cast<int>(a.x), static_cast<int>(a.y),
        static_cast<int>(b.x), static_cast<int>(b.y)};
}

/**
 * EFL_Vec2AddToRectFloat 将 a 和 b 合成为一个 SDL_FRect c。c 的前两个分量为 a 的值，后两个为 b 的值。
 * @param a 其中一个向量
 * @param b 另一个向量
 * @return a 和 b 合成的新向量
 */
inline SDL_FRect EFL_Vec2AddToRectFloat(const glm::vec2 a, const glm::vec2 b) {
    return {a.x, a.y, b.x, b.y};
}

#endif //ESCAPE_FROM_LILY_DEF_H
