//
// Created by MeowWow520 on 2026/6/2.
//

#include "nlohmann/json.hpp"
#include "ConfigManager.h"
#include <fstream>

#include "../Factory/EntityTypes.h"
#include "../Logger/Log.h"

using json = nlohmann::json;

int ConfigManager::Initialize() {
    std::ifstream ifsFont("assets/json/fonts.json");
    if (!ifsFont.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open {} failed", "assets/json/fonts.json");
        return -1;
    }
    json configFont = json::parse (ifsFont);
    if (configFont.is_null()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is null ");
        return -1;
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open {} successful", "assets/json/fonts.json");
    if (!configFont.is_object()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is not a object");
        return -1;
    }

    // 遍历
    for (auto it = configFont.begin(); it != configFont.end(); ++it) {
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
    ifsFont.close();
    configFont.clear();
    // ----------------------------
    std::ifstream ifsDefault("assets/json/fonts.json");
    if (!ifsDefault.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open {} failed", "assets/json/fonts.json");
        return -1;
    }
    json configDefault = json::parse (ifsDefault);
    if (configDefault.is_null()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is null ");
        return -1;
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open {} successful", "assets/json/fonts.json");
    if (!configDefault.is_object()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Load json file failed: json is not a object");
        return -1;
    }

    m_defaultJson =
    {
        {
            configDefault["WindowParams"]["title"],
            {configDefault["WindowParams"]["window_size"][0], configDefault["WindowParams"]["window_size"][0]},
            configDefault["WindowParams"]["FPS"]
        },
        {
            configDefault["PLayerParams"]["player_name"],
            {configDefault["PLayerParams"]["scale"][0], configDefault["PLayerParams"]["scale"][1]},
            {configDefault["PLayerParams"]["pivot"][0], configDefault["PLayerParams"]["pivot"][1]},
            configDefault["PLayerParams"]["max_speed"],
            configDefault["PLayerParams"]["acceleration"],
            configDefault["PLayerParams"]["health"],
            configDefault["PLayerParams"]["visible"],
            configDefault["PLayerParams"]["rotation"],
            configDefault["PLayerParams"]["color"],
            configDefault["PLayerParams"]["display_time"],
            SDL_Rect{
                configDefault["PLayerParams"]["rect"][0],
                configDefault["PLayerParams"]["rect"][1],
                configDefault["PLayerParams"]["rect"][2],
                configDefault["PLayerParams"]["rect"][3]
            }
        },
        {
            configDefault["CameraParams"]["name"]
        }
    };
    ifsDefault.close();
    configDefault.clear();

    return 0;
}

int ConfigManager::Quit() {
    m_defaultJson = {};
    m_fontJsonMap = {};
    return 0;
}

DefaultJson ConfigManager::getDefaultJson() const {
    return m_defaultJson;
}

std::vector<FontJson> ConfigManager::getFontJson() const {
    return m_fontJsonMap;
}
