//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGMANAGER_H
#define ESCAPE_FROM_LILY_CONFIGMANAGER_H

#include "Structs/BaseJson.h"


class ConfigManager {
    public:
        static ConfigManager& GetInstance() {
            static ConfigManager instance;
            return instance;
        }
        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        int InitDefaultJsonFile(const std::string& filePath);
        int InitPlayerJsonFile(const std::string& filePath);
        int InitFontsJsonFile(const std::string& filePath);
        int Initialize();
        int Quit();

        DefaultJson GetDefaultJson();
        PlayerJson GetPlayerJson();

    private:
        ConfigManager() = default;
        DefaultJson m_defaultJson{};
        PlayerJson m_playerJson{};
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
