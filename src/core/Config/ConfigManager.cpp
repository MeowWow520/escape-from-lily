//
// Created by MeowWow520 on 2026/6/2.
//

#include "ConfigManager.h"

#include <fstream>

#include "nlohmann/json.hpp"
#include "../Logger/Log.h"

using json = nlohmann::json;


int ConfigManager::Initialize() {
    if (LoadAllFiles() == -1)
        return -1;
    return 0;
}

int ConfigManager::Quit() {
    EFL_CHECK(LogCategory::Core, CleanFiles() == 0, "ConfigManager::CleanFiles");
    return 0;
}

int ConfigManager::LoadFile(const std::string& category, const std::string &path) {
    if (path.empty() || path[0] == '\0')
        return -1;
    std::ifstream ifs(path);
    m_files.insert({category, std::move(ifs)});

    return 0;
}

int ConfigManager::LoadAllFiles() {
    EFL_CHECK(LogCategory::Core,
        LoadFile("default", "assets/json/default.json"),
        "Load default.json");
    EFL_CHECK(LogCategory::Core,
        LoadFile("player", "assets/json/player.json"),
        "Load player.json")
    return 0;
}

int ConfigManager::CleanFiles() {
    for (auto it = m_files.begin(); it != m_files.end(); ++it)
        m_files.erase(it);
    return 0;
}
