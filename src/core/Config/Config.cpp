//
// Created by MeowWow520 on 2026/6/2.
//

#include "Config.h"


nlohmann::json Config::LoadFile(const std::string& path) {
    // 1. 打开文件
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;
    nlohmann::json config;
    try {
    } catch (const nlohmann::json::parse_error& e) {
        return 1;
    }
    return nullptr;
}
