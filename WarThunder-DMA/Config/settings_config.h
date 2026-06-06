#pragma once
#pragma once
#include "Config/lib/json.hpp"

using json = nlohmann::json;

struct KeyBinding {
    int virtualKey = 0;
    std::string keyName = "None";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KeyBinding, virtualKey, keyName);
class SettingsConfig
{
    std::string ConfigName;

public:
    SettingsConfig(const std::string& name)
    {
        ConfigName = name;
    }
    //vars
    std::string kmbox_ip = "";
    std::string kmbox_port = "";
    std::string kmbox_uuid = "";


    json ToJson()
    {
        json j;
        j[ConfigName]["kmbox_ip"] = kmbox_ip;
        j[ConfigName]["kmbox_port"] = kmbox_port;
        j[ConfigName]["kmbox_uuid"] = kmbox_uuid;
        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains("kmbox_ip"))
        {
            kmbox_ip = j[ConfigName]["kmbox_ip"];
        }
        if (j[ConfigName].contains("kmbox_port"))
        {
            kmbox_port = j[ConfigName]["kmbox_port"];
        }
        if (j[ConfigName].contains("kmbox_uuid"))
        {
            kmbox_uuid = j[ConfigName]["kmbox_uuid"];
        }

    }




};