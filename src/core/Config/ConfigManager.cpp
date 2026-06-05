//
// Created by MeowWow520 on 2026/6/2.
//

#include "ConfigManager.h"

#include <fstream>
#include "nlohmann/json.hpp"
#include "../Logger/Log.h"

using json = nlohmann::json;



PlayerJson ConfigManager::GetPlayer() {
    std::ifstream ifs("assets/json/player.json");
    if (!ifs.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Open assets/json/player.json");
        return PlayerJson{};
    }
    EFL_LOGGER_INFO(LogCategory::Core, "Open assets/json/player.json");
    json config = json::parse(ifs.get());
    const PlayerJson ret = {
        config["default_name"],
        config["texture_path"]
    };
    ifs.close();
    config.clear();
    return ret;
}
