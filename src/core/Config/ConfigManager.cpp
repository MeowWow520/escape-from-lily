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
    if (!ifs.is_open())
        return -1;
    m_files.insert({category, std::move(ifs)});
    return 0;
}

int ConfigManager::LoadAllFiles() {
    EFL_CHECK(LogCategory::Core,
        !LoadFile("default", "assets/json/default.json"),
        "Load default.json");
    EFL_CHECK(LogCategory::Core,
        !LoadFile("player", "assets/json/player.json"),
        "Load player.json")
    return 0;
}

int ConfigManager::CleanFiles() {
    m_files.clear();
    return 0;
}

Display ConfigManager::GetDisplay() {
    const auto it = m_files.find("default");
    if (it == m_files.end() || !it->second.is_open()) {
        EFL_LOGGER_ERROR(LogCategory::Core, "Couldn't find m_file: default");
        return Display{};
    }
    // FIXME: Clangd
    // Clangd: In template: constexpr variable '_Is_pointer_address_convertible<nlohmann::basic_json<>, nlohmann::basic_json<>>'
    // must be initialized by a constant expression

    json config = json::parse(it->second);
    const Display ret = {
        config["display"]["fps"],
        {
            glm::vec2(config["display"]["window"]["size"][0], config["display"]["window"]["size"][1]),
            config["display"]["window"]["title"]
        }
    };
    // FIXME: warning C4172: returning address of local variable or temporary : ret
    return ret;
}