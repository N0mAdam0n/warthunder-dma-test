#pragma once
#include <cstdint>
#include "../CUnitInfo/CUnitInfo.h"
#include "../CPlayer/CPlayer.h"
#include "../../Memory/Memory.h"
#include "../offsets.h"

class CUnit
{
public:
	std::uintptr_t base_address = 0;
	Vector3 position;
	Vector3 boundsmin;
	Vector3 boundsmax;
	bool invul_state = false;
	std::uint8_t team_num = 0;
	std::uint16_t unit_state = 0;
	CUnitInfo unit_info;
	CPlayer player;

	explicit CUnit(const std::uintptr_t addr = 0) : base_address(addr) {}

	void read_position_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::position, &position);
	}
	void read_boundsmin_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::boundsmin, &boundsmin);
	}
	void read_boundsmax_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::boundsmax, &boundsmax);
	}
	void read_invul_state_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::invulnerable_offset, &invul_state);
	}
	void read_team_num_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::team_num, &team_num);
	}
	void read_unit_state_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
		mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::unit_state, &unit_state);
	}
	void read_unit_info() {
		unit_info = CUnitInfo(mem.Read<std::uintptr_t>(base_address + offsets::_CUnit::unit_info));
	}
	void read_player() {
		player = CPlayer(mem.Read<std::uintptr_t>(base_address + offsets::_CUnit::player_ptr));
	}

	bool is_valid() const { return base_address != 0; }

	bool is_alive() const {
		if (unit_state >= 2)
			return false;
		if (player.gui_state != GuiState::ALIVE)
			return false;
		return true;
	}
};