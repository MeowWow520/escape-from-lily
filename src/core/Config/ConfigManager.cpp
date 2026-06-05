//
// Created by MeowWow520 on 2026/6/2.
//

#include "ConfigManager.h"

#include <fstream>

#include "nlohmann/json.hpp"
#include "../Logger/Log.h"

using json = nlohmann::json;


Display ConfigManager::GetDisplay() {
    std::fstream ifs("assets/json/default.json");
    if (!ifs.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open assets/json/default.json");
        return Display{};
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open assets/json/default.json");
    json config;
    // FIXME:
    ifs >> config;
    const Display ret = {
        config["display"]["fps"],
        {
            glm::vec2(config["display"]["window"]["size"][0], config["display"]["window"]["size"][1]),
            config["display"]["window"]["title"]
        }
    };
    ifs.close();
    config.clear();
    return ret;
}