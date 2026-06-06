#include "pch.h"
#include "Warthunder.h"
#include "../memory/Memory.h"
#include "offsets.h"


#include "CGame/CGame.h"

//std::mutex Warthunder::unit_list_mutex;
//std::vector<CUnit> Warthunder::unit_list;

auto Warthunder::rangle() -> const RAngle
{
	return mem.Read<RAngle>(globals::game_base + offsets::YawOffset);
}
auto Warthunder::rangle(const RAngle angle) -> void
{
	mem.Write<RAngle>(globals::game_base + offsets::YawOffset, angle);
}

