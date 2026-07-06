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
    const float max_speed =  config["max_speed"];
    m_playerJson = {
        default_name,
        texture_path,
        max_speed
    };

    ifs.close();
    config.clear();
    return 0;
}

int ConfigManager::InitFontsJsonFile(const std::string &filePath) {
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

    if (!config.is_object()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is not a object");
        return -1;
    }
    // 遍历
    for (auto it = config.begin(); it != config.end(); ++it) {
        const auto& obj = it.value();

        FontJson entry;
        entry.text          = it.key();
        entry.stype         = obj.value("stype", std::string("Static"));
        entry.font_size     = obj.value("font_size", 14.0f);
        entry.font_path     = obj.value("font_path", std::string(""));
        entry.color         = glm::vec4(
            obj["color"][0], obj["color"][1],
            obj["color"][2], obj["color"][3]);
        entry.rotation      = obj.value("rotation", 0.0f);
        entry.pivot         = glm::vec2(obj["pivot"][0], obj["pivot"][1]);
        entry.scale         = glm::vec2(obj["scale"][0], obj["scale"][1]);
        entry.display_time  = obj.value("display_time", -1.0f);

        m_fontJsonMap.push_back(entry);
    }

    ifs.close();
    config.clear();
    return 0;
}

int ConfigManager::Initialize() {
    EFL_CHECK(LogCategory::Core, !InitDefaultJsonFile("assets/json/default.json"), "InitDefaultJsonFile");
    EFL_CHECK(LogCategory::Core, !InitPlayerJsonFile("assets/json/player.json"),   "InitPlayerJsonFile" );
    EFL_CHECK(LogCategory::Core, !InitFontsJsonFile("assets/json/fonts.json"),     "InitFontsJsonFile"  );
    return 0;
}

int ConfigManager::Quit() {
    m_defaultJson = {};
    m_playerJson = {};
    m_fontJsonMap = {};
    return 0;
}

DefaultJson ConfigManager::GetDefaultJson() {
    return m_defaultJson;
}

PlayerJson ConfigManager::GetPlayerJson() {
    return m_playerJson;
}

std::vector<FontJson> ConfigManager::GetFontJson() {
    return m_fontJsonMap;
}

