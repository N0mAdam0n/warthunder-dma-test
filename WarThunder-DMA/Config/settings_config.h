#pragma once
#include "Config/lib/json.hpp"

using json = nlohmann::json;

class SettingsConfig
{
    std::string ConfigName;

public:
    SettingsConfig(const std::string& name)
    {
        ConfigName = name;
    }

    json ToJson()
    {
        json j;
        return j;
    }

    void FromJson(const json& j)
    {
        (void)j;
    }
};