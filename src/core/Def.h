//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_DEF_H
#define ESCAPE_FROM_LILY_DEF_H
#include <string>
#include <cmath>

#include <source_location>
#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>


// 是否开启加速功能
constexpr bool SWITCHER_ACCELERATION = false;
// 是否在 log 打印按键的状态
constexpr bool SWITCHER_KEYLOGGING = false;

// 实体默认最大速度
#define DEFAULT_MAX_SPEED    1500.0f
// 实体默认最大加速度
#define DEFAULT_ACCELERATION 50.0f

// FIXME: 这些转义字符在其他平台不可用，仅 Windows PowerShell 可用
#define CLR_RESET  "\033[0m"
#define CLR_RED    "\033[31m"
#define CLR_GREEN  "\033[32m"
#define CLR_YELLOW "\033[33m"
#define CLR_BLUE   "\033[34m"
#define CLR_CYAN   "\033[36m"
/**
 * 将 RGBA 颜色值分开转换为 4 位数值
 *
 * 如 COLOR(0xFF00FF00) --> 255, 0, 255, 0
 * @param HEX RGBA 颜色值
 */
#define COLOR(HEX) (\
    ( (HEX) >> 24 ) & 0xFF), \
    (((HEX) >> 16 ) & 0xFF), \
    (((HEX) >> 8  ) & 0xFF), \
    ( (HEX)         & 0xFF)

/**
 * SDL_LibInitChecker 检查 bool 形式的标志是否为真，并为其额外配备 log 功能
 *
 * @param flag SDL 库初始化函数
 * @param name SDL 库初始化函数的名字
 * @return 初始化成功返回 false，失败返回 true
 */
inline bool SDL_LibInitChecker(const bool flag, std::string name) {
    if (!flag) {
        spdlog::error("{} initialization failed", name);
        return true;
    } spdlog::info("{} initialization successfully", name);
    return false;
}

using ssl = std::source_location;

/**
 * EFL_ClassInit 为类内的初始化返回值进行 log 输出。
 *
 * @param flag 返回值
 * @param location 所在源码的定位
 * @return flag
 */
inline int EFL_ClassInit(const int flag, const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src") + 4;
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        spdlog::error("{}[{}:{}] {}{}{} initialization failed",
            CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
        return flag;
    }
    spdlog::info("{}[{}:{}] {}{}{} initialization successfully",
        CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
    return 0;
}
/**
 * EFL_ClassQuit 为类内的清理返回值进行 log 输出。
 *
 * @param flag 返回值
 * @param location 所在源码的定位
 * @return flag
 */
inline int EFL_ClassQuit(const int flag,  const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src") + 4;
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        spdlog::error("{}[{}:{}] {}{}{} quit failed",
            CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
        return flag;
    }
    spdlog::info("{}[{}:{}] {}{}{} quit successfully",
        CLR_BLUE, c_file_name, location.line(), CLR_YELLOW, c_function_name, CLR_RESET);
    return 0;
}

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
