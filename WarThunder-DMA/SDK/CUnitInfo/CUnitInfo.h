#pragma once
#include <cstdint>
#include "../CVehicleInfo/CVehicleInfo.h"
#include "../../Memory/Memory.h"
#include "../offsets.h"

class CUnitInfo
{
public:
	std::uintptr_t base_address = 0;
	CVehicleInfo vehicle_info;

	explicit CUnitInfo(const std::uintptr_t addr = 0) : base_address(addr) {}

	void set_vehicle_info() {
		vehicle_info = CVehicleInfo(mem.Read<std::uintptr_t>(base_address + offsets::_CUnitInfo::tank_info));
	}

	bool is_valid() const { return base_address != 0; }
};