#pragma once
#include <cstdint>

enum GuiState : std::uint8_t
{
	NONE = 0,
	MENU = 1,
	ALIVE = 2,
	DEAD = 3,
	SPEC = 6,
	SPAWN_MENU = 8,
	LOADING = 10
};