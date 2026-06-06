#pragma once
#include <fstream> 


#include <settings_config.h>
#include "Player_ESP_config.h"
static std::unordered_map<int, std::string> keyNames = {
    // Function keys
    { VK_F1, "F1" },
    { VK_F2, "F2" },
    { VK_F3, "F3" },
    { VK_F4, "F4" },
    { VK_F5, "F5" },
    { VK_F6, "F6" },
    { VK_F7, "F7" },
    { VK_F8, "F8" },
    { VK_F9, "F9" },
    { VK_F10, "F10" },
    { VK_F11, "F11" },
    { VK_F12, "F12" },

    // Numeric keypad
    { VK_NUMPAD0, "Numpad 0" },
    { VK_NUMPAD1, "Numpad 1" },
    { VK_NUMPAD2, "Numpad 2" },
    { VK_NUMPAD3, "Numpad 3" },
    { VK_NUMPAD4, "Numpad 4" },
    { VK_NUMPAD5, "Numpad 5" },
    { VK_NUMPAD6, "Numpad 6" },
    { VK_NUMPAD7, "Numpad 7" },
    { VK_NUMPAD8, "Numpad 8" },
    { VK_NUMPAD9, "Numpad 9" },
    { VK_MULTIPLY, "Numpad *" },
    { VK_ADD, "Numpad +" },
    { VK_SUBTRACT, "Numpad -" },
    { VK_DECIMAL, "Numpad ." },
    { VK_DIVIDE, "Numpad /" },

    // Letters
    { 0x41, "A" }, // VK_A
    { 0x42, "B" }, // VK_B
    { 0x43, "C" }, // VK_C
    { 0x44, "D" }, // VK_D
    { 0x45, "E" }, // VK_E
    { 0x46, "F" }, // VK_F
    { 0x47, "G" }, // VK_G
    { 0x48, "H" }, // VK_H
    { 0x49, "I" }, // VK_I
    { 0x4A, "J" }, // VK_J
    { 0x4B, "K" }, // VK_K
    { 0x4C, "L" }, // VK_L
    { 0x4D, "M" }, // VK_M
    { 0x4E, "N" }, // VK_N
    { 0x4F, "O" }, // VK_O
    { 0x50, "P" }, // VK_P
    { 0x51, "Q" }, // VK_Q
    { 0x52, "R" }, // VK_R
    { 0x53, "S" }, // VK_S
    { 0x54, "T" }, // VK_T
    { 0x55, "U" }, // VK_U
    { 0x56, "V" }, // VK_V
    { 0x57, "W" }, // VK_W
    { 0x58, "X" }, // VK_X
    { 0x59, "Y" }, // VK_Y
    { 0x5A, "Z" }, // VK_Z

    // Numbers (main keyboard)
    { 0x30, "0" }, // VK_0
    { 0x31, "1" }, // VK_1
    { 0x32, "2" }, // VK_2
    { 0x33, "3" }, // VK_3
    { 0x34, "4" }, // VK_4
    { 0x35, "5" }, // VK_5
    { 0x36, "6" }, // VK_6
    { 0x37, "7" }, // VK_7
    { 0x38, "8" }, // VK_8
    { 0x39, "9" }, // VK_9

    // Special characters
    { VK_OEM_1, ";" },      // ;:
    { VK_OEM_PLUS, "=" },   // =+
    { VK_OEM_COMMA, "," },  // ,<
    { VK_OEM_MINUS, "-" },  // -_
    { VK_OEM_PERIOD, "." }, // .>
    { VK_OEM_2, "/" },      // /?
    { VK_OEM_3, "`" },      // `~
    { VK_OEM_4, "[" },      // [{
    { VK_OEM_5, "\\" },     // \|
    { VK_OEM_6, "]" },      // ]}
    { VK_OEM_7, "'" },      // '"

    // Main control keys
    { VK_SPACE, "Space" },
    { VK_RETURN, "Enter" },
    { VK_ESCAPE, "Esc" },
    { VK_TAB, "Tab" },
    { VK_BACK, "Backspace" },
    { VK_CAPITAL, "Caps Lock" },

    // Modifier keys
    { VK_SHIFT, "Shift" },
    { VK_CONTROL, "Ctrl" },
    { VK_MENU, "Alt" },
    { VK_LSHIFT, "L Shift" },
    { VK_RSHIFT, "R Shift" },
    { VK_LCONTROL, "L Ctrl" },
    { VK_RCONTROL, "R Ctrl" },
    { VK_LMENU, "L Alt" },
    { VK_RMENU, "R Alt" },
    { VK_LWIN, "L Win" },
    { VK_RWIN, "R Win" },
    { VK_APPS, "Menu" },

    // Navigation keys
    { VK_INSERT, "Insert" },
    { VK_DELETE, "Delete" },
    { VK_HOME, "Home" },
    { VK_END, "End" },
    { VK_PRIOR, "Page Up" },
    { VK_NEXT, "Page Down" },
    { VK_LEFT, "Left" },
    { VK_RIGHT, "Right" },
    { VK_UP, "Up" },
    { VK_DOWN, "Down" },

    // System keys
    { VK_SNAPSHOT, "Print Screen" },
    { VK_SCROLL, "Scroll Lock" },
    { VK_PAUSE, "Pause" },

    // Multimedia keys
    { VK_VOLUME_MUTE, "Volume Mute" },
    { VK_VOLUME_DOWN, "Volume Down" },
    { VK_VOLUME_UP, "Volume Up" },
    { VK_MEDIA_NEXT_TRACK, "Next Track" },
    { VK_MEDIA_PREV_TRACK, "Previous Track" },
    { VK_MEDIA_STOP, "Stop" },
    { VK_MEDIA_PLAY_PAUSE, "Play/Pause" },

    // Additional keys
    { VK_NUMLOCK, "Num Lock" },
    { VK_CLEAR, "Clear" },
};
class ConfigInstances
{
private:
public:
	//all config class

    SettingsConfig  Settings = SettingsConfig("Settings");
    Player_ESP_config Player_ESP = Player_ESP_config("Player_ESP");

	json ToJson()
	{
		json jsoned;
		//all config vars
        jsoned.merge_patch(Settings.ToJson());
        jsoned.merge_patch(Player_ESP.ToJson());

		return jsoned;
	}

	void FromJson(json jsoned)
	{
		//all config vars
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
			std::cout << "Config saved\n";
		}
		else {
			std::cerr << "Failed to open file for writing\n";
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
				std::cout << "Config loaded\n";
			}
			catch (const json::parse_error& e) {
				std::cerr << "Error parsing JSON\n";
			}
			file.close();
		}
		else {
			std::cerr << "Failed to open file for reading\n" ;
		}
	};
};
inline ConfigInstances ConfigInstance;