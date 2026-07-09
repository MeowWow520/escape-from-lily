//
// Created by MeowWow520 on 2026/6/2.
//

#include "nlohmann/json.hpp"
#include "ConfigManager.h"
#include <fstream>
#include "../Logger/Log.h"

using json = nlohmann::json;



int ConfigManager::initDefaultJsonFile(const std::string &filePath) {
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

    j_PlayerParams jPlayer_params = {
        config["PLayerParams"]["player_params"],
        {config["PLayerParams"]["scale"][0],config["PLayerParams"]["scale"][1]},
        {config["PLayerParams"]["pivot"][0], config["PLayerParams"]["pivot"][1]},
        config["PLayerParams"]["max_speed"],
        config["PLayerParams"]["acceleration"],
        config["PLayerParams"]["health"],
        config["PLayerParams"]["visible"],
        config["PLayerParams"]["rotation"],
        config["PLayerParams"]["color"],
        config["PLayerParams"]["blend_mode"],
        config["PLayerParams"]["rect"],
    };
    j_CameraParams jCamera_params = {
        config["PLayerParams"]["camera_params"]
    };

    m_defaultJson = { jPlayer_params, jCamera_params };

    ifs.close();
    config.clear();
    return 0;
}

int ConfigManager::initFontsJsonFile(const std::string &filePath) {
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
    EFL_CHECK(LogCategory::Core, !initDefaultJsonFile("assets/json/default.json"), "initDefaultJsonFile");
    EFL_CHECK(LogCategory::Core, !initFontsJsonFile("assets/json/fonts.json"),     "initFontsJsonFile"  );
    return 0;
}

int ConfigManager::Quit() {
    m_defaultJson = {};
    m_fontJsonMap = {};
    return 0;
}

DefaultJson ConfigManager::getDefaultJson() {
    return m_defaultJson;
}

std::vector<FontJson> ConfigManager::getFontJson() {
    return m_fontJsonMap;
}

