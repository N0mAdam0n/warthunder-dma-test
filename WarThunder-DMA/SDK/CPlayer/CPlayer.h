#pragma once
#include <cstdint>
#include "../Structs/Structs.h"

class CUnit;

class CPlayer
{
public:
	explicit CPlayer(const std::uintptr_t addr = 0) : base_address(addr) {}

	const std::uintptr_t operator=(const std::uintptr_t addr) {
		return this->base_address = addr;
	}

	GuiState gui_state = GuiState::NONE;

	auto unit() -> CUnit;
	static auto get_local() -> const std::uintptr_t;
	auto is_valid() const -> bool;
	void read_gui_state();

private:
	std::uintptr_t base_address = 0;
};