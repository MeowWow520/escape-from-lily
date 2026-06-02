//
// Created by MeowWow520 on 2026/6/2.
//

#ifndef ESCAPE_FROM_LILY_CONFIG_H
#define ESCAPE_FROM_LILY_CONFIG_H
#include <fstream>
#include "nlohmann/json.hpp"

class Config {
    public:
        Config() = default;
        ~Config() = default;

        Config(const Config&) = default;
        Config& operator=(const Config&) = default;

        static nlohmann::json LoadFile(const std::string& path);
};


#endif //ESCAPE_FROM_LILY_CONFIG_H
