#pragma once
#include <fstream>
#include "RunLogger.h"

#include <settings_config.h>
#include "Player_ESP_config.h"

class ConfigInstances
{
private:
public:
    SettingsConfig  Settings = SettingsConfig("Settings");
    Player_ESP_config Player_ESP = Player_ESP_config("Player_ESP");

	json ToJson()
	{
		json jsoned;
        jsoned.merge_patch(Settings.ToJson());
        jsoned.merge_patch(Player_ESP.ToJson());

		return jsoned;
	}

	void FromJson(json jsoned)
	{
        Settings.FromJson(jsoned);
        Player_ESP.FromJson(jsoned);
	}

	void SaveToFile(const std::string& filename)
	{
		json jsoned = ToJson();
		std::ofstream file(filename);
		if (file.is_open()) {
			file << jsoned.dump(4);
			file.close();
			RunLogger::Info("配置已保存: %s", filename.c_str());
		}
		else {
			RunLogger::Error("无法写入配置文件: %s", filename.c_str());
		}
	};
	void LoadFromFile(const std::string& filename)
	{
		std::ifstream file(filename);
		if (file.is_open()) {
			json json;
			try {
				file >> json; 
				FromJson(json);
				RunLogger::Info("配置已加载: %s", filename.c_str());
			}
			catch (const json::parse_error& e) {
				RunLogger::Error("配置文件 JSON 解析失败: %s", filename.c_str());
			}
			file.close();
		}
		else {
			RunLogger::Warn("配置文件不存在或无法读取: %s（将使用默认值）", filename.c_str());
		}
	};
};
inline ConfigInstances ConfigInstance;