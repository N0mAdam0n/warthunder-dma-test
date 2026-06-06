#include "pch.h"
#include "CUnitInfo.h"
#include "../../memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"
//auto CUnitInfo::bombsight_local(const bool enabled) -> void
//{
//	mem.Write<bool>(this->base_address + offsets::_CUnitInfo::bombsight, enabled);
//}
//
//auto CUnitInfo::bombsight_local() -> const bool
//{
//	return mem.Read<bool>(this->base_address + offsets::_CUnitInfo::bombsight);
//}
//
//auto CUnitInfo::is_valid() -> const bool
//{
//	return this->base_address != 0;
//}
//
//auto CUnitInfo::unit_type() -> std::string
//{
//	return globals::ReadString(mem.Read<std::uintptr_t>(this->base_address + offsets::_CUnitInfo::unit_type),30);
//}
//
//auto CUnitInfo::vehicle_info () -> CVehicleInfo
//{
//	return CVehicleInfo(mem.Read<uintptr_t>(this->base_address + offsets::_CUnitInfo::tank_info));
//}
//
//auto CUnitInfo::is_plane() -> const bool
//{
//	auto unit_type = this->unit_type();
//	return (unit_type == "exp_bomber" || unit_type == "exp_assault" || unit_type == "exp_fighter");
//}
//
//auto CUnitInfo::is_dummy() -> const bool
//{
//	auto unit_type = this->unit_type();
//	return (unit_type == "exp_fortification" || unit_type == "exp_structure" || unit_type == "exp_aaa" || unit_type == "dummy_plane" || unit_type == "exp_bridge");
//}
//
//
