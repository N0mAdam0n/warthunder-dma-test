#include "pch.h"
#include "CPlayer.h"
#include "../../Memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"
#include "../CUnit/CUnit.h"

auto CPlayer::unit() -> CUnit
{
	return CUnit(mem.Read<std::uintptr_t>(base_address + offsets::_CPlayer::player_unit) - 1);
}

auto CPlayer::get_local() -> const std::uintptr_t
{
	return mem.Read<std::uintptr_t>(globals::game_base + offsets::m_player_ptr + offsets::m_player::local_player);
}

auto CPlayer::is_valid() const -> bool
{
	return this->base_address != 0;
}

void CPlayer::read_gui_state()
{
	gui_state = GuiState(mem.Read<uint8_t>(this->base_address + offsets::_CPlayer::gui_state));
}