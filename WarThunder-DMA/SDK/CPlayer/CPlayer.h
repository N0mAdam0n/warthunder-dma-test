#pragma once

#include <cstdint>
#include <string>
#include "../Structs/Structs.h"


class CUnit;// FUCKING C++ MODULES IDK HOW THIS WORKS I'M IN 8 CLASS HELP ME @Mevasik 2024


class CPlayer
{
public:

	CPlayer(const std::uintptr_t base_address = 0) : base_address(base_address) {}

	const std::uintptr_t operator=(const std::uintptr_t base_address) {
		return this->base_address = base_address;
	}

	const bool operator==(CPlayer player) {
		return this->base_address == player.base_address;
	}

	const bool operator==(CPlayer* player) {
		return this->base_address == player->base_address;
	}
	GuiState gui_state;
	std::string name;
	
	auto unit() -> CUnit;
	static auto get_local() -> const std::uintptr_t;
	auto is_valid() const -> bool;
	void read_gui_state();
	void set_name();
	/*void set_team();*/
private:
	std::uintptr_t base_address;
};

