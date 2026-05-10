//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_DEF_H
#define ESCAPE_FROM_LILY_DEF_H

/**
 * 将 RGBA 颜色值分开转换为 4 位数值
 * 如 COLOR(0xFF00FF00) --> 255, 0, 255, 0
 * @param hex RGBA 颜色值
 */
#define COLOR(hex) (\
    ( (hex) >> 24 ) & 0xFF), \
    (((hex) >> 16 ) & 0xFF), \
    (((hex) >> 8  ) & 0xFF), \
    ( (hex) & 0xFF)

#endif //ESCAPE_FROM_LILY_DEF_H
