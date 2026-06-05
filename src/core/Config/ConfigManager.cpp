//
// Created by MeowWow520 on 2026/6/2.
//

#include "nlohmann/json.hpp"
#include "ConfigManager.h"
#include <fstream>
#include "../Logger/Log.h"

using json = nlohmann::json;



int ConfigManager::InitDefaultJsonFile(const std::string &filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open {} failed", filePath);
        return -1;
    }
    json config = json::parse (ifs);

    if (config.is_null()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is null ");
        return -1;
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open {} successful", filePath);


    const Uint32 fps = config["display"]["fps"];
    const auto window_size = glm::vec2(config["display"]["window"]["size"][0],
        config["display"]["window"]["size"][1]);
    const std::string window_title = config["display"]["window"]["title"];
    const bool acceleration = config["feature"]["acceleration"];
    const bool key_logging = config["feature"]["key-logging"];
    const std::string default_color = config["font"]["default_color"];
    const std::string notosansc = config["font"]["NotoSanSc"];

    m_defaultJson = {
        {fps, {window_size, window_title}},
        {acceleration, key_logging},
        {default_color, notosansc},
    };

    ifs.close();
    config.clear();
    return 0;
}

int ConfigManager::InitPlayerJsonFile(const std::string &filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open {} failed", filePath);
        return -1;
    }
    json config = json::parse (ifs);

    if (config.is_null()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is null ");
        return -1;
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open {} successful", filePath);

    const std::string default_name = config["default_name"];
    const std::string texture_path = config["texture_path"];

    m_playerJson = {
        default_name,
        texture_path,
    };

    ifs.close();
    config.clear();
    return 0;
}

int ConfigManager::Initialize() {
    EFL_CHECK(LogCategory::Core, InitDefaultJsonFile("assets/json/default.json"), "InitDefaultJsonFile");
    EFL_CHECK(LogCategory::Core, InitPlayerJsonFile("assets/json/player.json"),   "InitPlayerJsonFile" );
    return 0;
}

DefaultJson ConfigManager::GetDefaultJson() {
    return m_defaultJson;
}

PlayerJson ConfigManager::GetPlayerJson() {
    return m_playerJson;
}

