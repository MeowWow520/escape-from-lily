//
// Created by MeowWow520 on 2026/6/2.
//

#include "ConfigManager.h"

#include <fstream>

#include "nlohmann/json.hpp"
#include "../Logger/Log.h"

using json = nlohmann::json;


int ConfigManager::Initialize() {
    return 0;
}

int ConfigManager::Quit() {
    return 0;
}

bool ConfigManager::LoadFile(std::string category, const std::string &path) {
    if (path == "" || path.empty())
        return false;
    std::ifstream ifs(path);
    m_files.insert({category, std::move(ifs)});

    return true;
}

bool ConfigManager::LoadAllFiles(std::string &path) {
    EFL_CHECK(LogCategory::Core,
        LoadFile("default", "assets/json/default.json"),
        "Load default.json");
    EFL_CHECK(LogCategory::Core,
        LoadFile("player", "assets/json/player.json"),
        "Load player.json")
    return true;
}
