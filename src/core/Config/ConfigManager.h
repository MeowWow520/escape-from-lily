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

        int Initialize();
        int Quit();

        [[nodiscard]] DefaultJson getDefaultJson() const;

        [[nodiscard]] std::vector<FontJson> getFontJson() const;

    protected:
        ConfigManager() = default;
        DefaultJson m_defaultJson{};
        std::vector<FontJson> m_fontJsonMap{};
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
