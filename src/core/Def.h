//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_DEF_H
#define ESCAPE_FROM_LILY_DEF_H
#include <string>
#include <source_location>
#include <spdlog/spdlog.h>
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
inline int EFL_ClassInit(const int flag, const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src");
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        spdlog::error("[{}:{}] {} initialization failed", c_file_name, location.line(), c_function_name);
        return -1;
    }
    spdlog::info("[{}:{}] {} initialization successfully", c_file_name, location.line(), c_function_name);
    return 0;
}

inline int EFL_ClassQuit(const int flag,  const ssl &location) {
    const char* c_file_name = std::strstr(location.file_name(), "src");
    const char* c_function_name = location.function_name() + 12;
    if (flag != 0) {
        spdlog::error("[{}:{}] {} quit failed", c_file_name, location.line(), c_function_name);
        return -1;
    }
    spdlog::info("[{}:{}] {} quit successfully", c_file_name, location.line(), c_function_name);
    return 0;
}

#endif //ESCAPE_FROM_LILY_DEF_H
