//
// Created by MeowWow520 on 2026/5/19.
//

#ifndef ESCAPE_FROM_LILY_LOG_H
#define ESCAPE_FROM_LILY_LOG_H
#include <spdlog/spdlog.h>
#include <string>
#include "LogCategory.h"



namespace EFL {
    struct LogConfig {
        std::string file_path = "logs/escape-from-lily.log";  // 日志文件的路径
        bool truncate_log_file = true;                        // 截断旧日志
        bool console_logging = true;                          // 输出控制台
        bool enable_logging = true;                           // 输出文件
    };

    const char* GetCategoryName(LogCategory cate) noexcept;

    void RegisterLogCategory(const LogConfig& config = {});

    void QuitLogger();
} // Escape from lily


#define EFL_LOGGER_INFO(cat, ...)      \
    spdlog::get(EFL::GetCategoryName(cat))->info(__VA_ARGS__)

#define EFL_LOGGER_ERROR(cat, ...)     \
    spdlog::get(EFL::GetCategoryName(cat))->error(__VA_ARGS__)

#define EFL_LOG_ENTITY_CREATED(name)   \
    EFL_LOGGER_INFO(LogCategory::Entity, "{} object constructed", name)

#define EFL_LOG_ENTITY_DESTROYED(name) \
    EFL_LOGGER_INFO(LogCategory::Entity, "{} object destroyed", name)



#endif //ESCAPE_FROM_LILY_LOG_H
