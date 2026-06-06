#include "pch.h"
#include "CHud.h"
#include "../../memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"

auto CHud::get_instance() -> const bool
{
	return (this->base_address = mem.Read<std::uintptr_t>(globals::game_base + offsets::CHud)) != 0;
}

auto CHud::penetration_crosshair(const bool enabled) -> void
{
	mem.Write<bool>(this->base_address + offsets::_CHud::arcade_crosshair, enabled);
}

auto CHud::penetration_crosshair() -> const bool
{
	return mem.Read<bool>(this->base_address + offsets::_CHud::arcade_crosshair);
}

auto CHud::can_select_unit(const bool enabled) -> void
{
	mem.Write<bool>(this->base_address + offsets::_CHud::can_select_unit, enabled);
}

auto CHud::can_select_unit() -> const bool
{
	return mem.Read<bool>(this->base_address + offsets::_CHud::can_select_unit);
}

auto CHud::aircraft_selection(const bool enabled) -> void
{
	mem.Write<bool>(this->base_address + offsets::_CHud::aircraft_distance, enabled);
}

auto CHud::aircraft_selection() -> const bool
{
	return mem.Read<bool>(this->base_address + offsets::_CHud::aircraft_distance);
}

auto CHud::ground_to_air_prediction(const bool enabled) -> void
{
	mem.Write<bool>(this->base_address + offsets::_CHud::ground_to_air_prediction, !enabled); // false to true???
}

auto CHud::ground_to_air_prediction() -> const bool
{
	return !mem.Read<bool>(this->base_address + offsets::_CHud::ground_to_air_prediction);
}

auto CHud::vehicle_names(const bool enabled) -> void
{
	mem.Write<std::uint8_t>(this->base_address + offsets::_CHud::tank_esp, enabled ? 2 : 0);
}

auto CHud::vehicle_names() -> const bool
{
	return mem.Read<std::uint8_t>(this->base_address + offsets::_CHud::tank_esp) == 2;
}
