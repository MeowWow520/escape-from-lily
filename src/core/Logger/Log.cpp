//
// Created by MeowWow520 on 2026/5/19.
//

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace EFL {
    namespace {
        /**
         * CreateLogger 创建一个 log
         *
         * @param name 注册名
         * @param config 注册的设置
         */
        void CreateLogger(const char* name, const LogConfig& config) {
            std::vector<spdlog::sink_ptr> sinks;
            if (config.console_logging) {
                const auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
                console_sink->set_pattern("%^[%T.%e] [%=7n] [%l]%$ %v");
                sinks.push_back(console_sink);
            }
            if (config.enable_logging) {
                const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                    config.file_path, config.truncate_log_file);
                file_sink->set_pattern("[%Y-%m-%d %T.%e] [%=7n] [%l] %v");
                sinks.push_back(file_sink);
            }
            const auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::trace);
            spdlog::register_logger(logger);
        }
    }

    const char *GetCategoryName(const LogCategory cate) noexcept {
        switch (cate) {
            case LogCategory::Core:     return "Core";
            case LogCategory::Entity:   return "Entity";
            case LogCategory::Input:    return "Input";
            case LogCategory::Scene:    return "Scene";
            case LogCategory::Renderer: return "Renderer";
        }
        return "unknow";
    }
    void RegisterLogCategory(const LogConfig& config) {
        CreateLogger("Core", config);
        CreateLogger("Entity", config);
        CreateLogger("Input", config);
        CreateLogger("Scene", config);
        CreateLogger("Renderer", config);
        spdlog::info("Log system initialized, file: {}", config.file_path);
    }
    void QuitLogger() {
        spdlog::drop("Core");
        spdlog::drop("Entity");
        spdlog::drop("Input");
        spdlog::drop("Scene");
        spdlog::drop("Renderer");
    }
} // Escape from lily