//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGMANAGER_H
#define ESCAPE_FROM_LILY_CONFIGMANAGER_H

#include "Structs/BaseJson.h"


class ConfigManager {
    public:
        ConfigManager() = default;
        ~ConfigManager() = default;

        int InitDefaultJsonFile(const std::string& filePath);
        int InitPlayerJsonFile(const std::string& filePath);

        int Initialize();


        DefaultJson GetDefaultJson();
        PlayerJson GetPlayerJson();


    private:
        DefaultJson m_defaultJson{};
        PlayerJson m_playerJson{};
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
