#pragma once
#include "Config/lib/json.hpp"
#include "imgui.h"
using json = nlohmann::json;

class Player_ESP_config
{
	std::string ConfigName;
public:
	Player_ESP_config(const std::string& name) { ConfigName = name; }

	bool Enable = false;
	bool Enable_simple_box = false;
	ImU32 box_color = IM_COL32(255, 0, 0, 255);
	float box_thickness = 2.0f;
	float font_size = 15.0f;
	ImU32 text_color = IM_COL32(255, 250, 250, 255);
	bool Show_distanse = false;
	int Max_distance = 300;
	bool Show_unit_name = false;
	bool show_invul_state = false;

	json ToJson()
	{
		json j;
		j[ConfigName]["Enable"] = Enable;
		j[ConfigName]["Enable_simple_box"] = Enable_simple_box;
		j[ConfigName]["box_thickness"] = box_thickness;
		j[ConfigName]["box_color"] = box_color;
		j[ConfigName]["font_size"] = font_size;
		j[ConfigName]["text_color"] = text_color;
		j[ConfigName]["Show_distanse"] = Show_distanse;
		j[ConfigName]["Max_distance"] = Max_distance;
		j[ConfigName]["Show_unit_name"] = Show_unit_name;
		j[ConfigName]["show_invul_state"] = show_invul_state;
		return j;
	}

	void FromJson(const json& j)
	{
		if (!j.contains(ConfigName))
			return;
		if (j[ConfigName].contains("Enable")) Enable = j[ConfigName]["Enable"];
		if (j[ConfigName].contains("Enable_simple_box")) Enable_simple_box = j[ConfigName]["Enable_simple_box"];
		if (j[ConfigName].contains("box_color")) box_color = j[ConfigName]["box_color"];
		if (j[ConfigName].contains("box_thickness")) box_thickness = j[ConfigName]["box_thickness"];
		if (j[ConfigName].contains("font_size")) font_size = j[ConfigName]["font_size"];
		if (j[ConfigName].contains("text_color")) text_color = j[ConfigName]["text_color"];
		if (j[ConfigName].contains("Show_distanse")) Show_distanse = j[ConfigName]["Show_distanse"];
		if (j[ConfigName].contains("Max_distance")) Max_distance = j[ConfigName]["Max_distance"];
		if (j[ConfigName].contains("Show_unit_name")) Show_unit_name = j[ConfigName]["Show_unit_name"];
		if (j[ConfigName].contains("show_invul_state")) show_invul_state = j[ConfigName]["show_invul_state"];
	}
};