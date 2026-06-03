//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIGMANAGER_H
#define ESCAPE_FROM_LILY_CONFIGMANAGER_H
#include <fstream>
#include <string>
#include <unordered_map>



class ConfigManager {
    public:
        static ConfigManager& GetInstance() {
            static ConfigManager instance;
            return instance;
        }
        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        int Initialize();
        int Quit();

        int LoadFile(const std::string& category, const std::string& path);
        int LoadAllFiles();
        int CleanFiles();



    private:
        ConfigManager() = default;

        std::unordered_map<std::string, std::ifstream> m_files{};
};


#endif //ESCAPE_FROM_LILY_CONFIGMANAGER_H
