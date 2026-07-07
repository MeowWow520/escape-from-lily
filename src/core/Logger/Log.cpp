//
// Created by MeowWow520 on 2026/5/19.
//

#include "Log.h"

#include <filesystem>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace EFL {
    namespace {
        /**
         * CreateLogger 创建一个 log
         *
         * @param name 注册名
         * @param config 注册的设置
         * @param file_sink 共享的文件 sink
         */
        void CreateLogger(const char* name, const LogConfig& config,
                          const std::shared_ptr<spdlog::sinks::basic_file_sink_mt>& file_sink) {
            std::vector<spdlog::sink_ptr> sinks;
            if (config.console_logging) {
                const auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
                console_sink->set_pattern("%^[%T.%e] [%-7n] [%l]%$ %v");
                sinks.push_back(console_sink);
            }
            if (config.enable_logging && file_sink) {
                sinks.push_back(file_sink);
            }
            const auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::trace);
            spdlog::register_logger(logger);
        }
    }

    const char *getCategoryName(const LogCategory cate) noexcept {
        switch (cate) {
            case LogCategory::Core:     return "Core";
            case LogCategory::Entity:   return "Entity";
            case LogCategory::Input:    return "Input";
            case LogCategory::Scene:    return "Scene";
            case LogCategory::Renderer: return "Renderer";
            case LogCategory::Factory:  return "Factory";
            case LogCategory::Font:     return "Font";
        }
        return "unknow";
    }
    void registerLogCategory(const LogConfig& config) {
        // 创建日志目录（如果不存在）
        std::filesystem::create_directories(
            std::filesystem::path(config.file_path).parent_path()
        );

        // 文件 sink 只创建一次，所有 logger 共享
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;
        if (config.enable_logging) {
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                config.file_path, config.truncate_log_file);
            file_sink->set_pattern("[%Y-%m-%d %T.%e] [%-7n] [%l] %v");
        }

        CreateLogger("Core", config, file_sink);
        CreateLogger("Entity", config, file_sink);
        CreateLogger("Input", config, file_sink);
        CreateLogger("Scene", config, file_sink);
        CreateLogger("Renderer", config, file_sink);
        CreateLogger("Factory", config, file_sink);
        CreateLogger("Font", config, file_sink);
        spdlog::info("Log system initialized, file: {}", config.file_path);
    }
    void quitLogger() {
        spdlog::get("Core")->flush();
        spdlog::get("Entity")->flush();
        spdlog::get("Input")->flush();
        spdlog::get("Scene")->flush();
        spdlog::get("Renderer")->flush();
        spdlog::get("Factory")->flush();
        spdlog::get("Font")->flush();
        spdlog::drop("Core");
        spdlog::drop("Entity");
        spdlog::drop("Input");
        spdlog::drop("Scene");
        spdlog::drop("Renderer");
        spdlog::drop("Factory");
        spdlog::drop("Font");
    }
} // Escape from lily