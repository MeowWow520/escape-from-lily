//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGMANAGER_H
#define ESCAPE_FROM_LILY_CONFIGMANAGER_H


#include "ConfigCategory.h"


class ConfigManager {
    public:
        ConfigManager() = default;
        ~ConfigManager() = default;

        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        static DisplayJson GetDisplay();
        static PlayerJson GetPlayer();
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
