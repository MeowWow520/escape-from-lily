//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGMANAGER_H
#define ESCAPE_FROM_LILY_CONFIGMANAGER_H

#include "Structs/BaseJson.h"


class ConfigManager {
    public:
        static ConfigManager& getInstance() {
            static ConfigManager instance;
            return instance;
        }
        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        int initDefaultJsonFile(const std::string& filePath);
        int initPlayerJsonFile(const std::string& filePath);
        int initFontsJsonFile(const std::string& filePath);
        int Initialize();
        int Quit();

        DefaultJson getDefaultJson();
        PlayerJson getPlayerJson();
        std::vector<FontJson> getFontJson();

    private:
        ConfigManager() = default;
        DefaultJson m_defaultJson{};
        PlayerJson m_playerJson{};
        std::vector<FontJson> m_fontJsonMap{};
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
